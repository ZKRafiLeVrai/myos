#include "vfs.h"
#include "string.h"
#include "vga.h"
#include "printk.h"

inode_t root_fs[MAX_FILES];

/* Simple cache for last accessed file */
typedef struct {
    char cached_name[MAX_FILENAME];
    int cached_index;
} vfs_cache_t;

static vfs_cache_t cache = {0};

void vfs_init() {
    // Initialisation du système de fichiers à vide
    for(int i = 0; i < MAX_FILES; i++) {
        root_fs[i].active = 0;
    }
    
    /* Initialize cache */
    cache.cached_index = -1;
    memset(cache.cached_name, 0, MAX_FILENAME);

    // Création d'un fichier de bienvenue (Message Of The Day)
    strcpy(root_fs[0].name, "motd.txt");
    strcpy(root_fs[0].content, "Bienvenue sur MyOS Linux Pro!\nSysteme en Mode Protege 32-bit.\n");
    root_fs[0].size = strlen(root_fs[0].content);
    root_fs[0].active = 1;

    printk(LOG_INFO, "VFS: Ramdisk initialise avec motd.txt");
}

void sys_ls() {
    kprint("Listing directory / :\n");
    for(int i = 0; i < MAX_FILES; i++) {
        if(root_fs[i].active) {
            kprint("- ");
            kprint(root_fs[i].name);
            kprint("\n");
        }
    }
}
void vfs_create_file(const char* filename, const char* content) {
    if (!filename || !content) {
        printk(LOG_ERR, "VFS: Invalid filename or content pointer.");
        return;
    }
    for(int i = 0; i < MAX_FILES; i++) {
        if(!root_fs[i].active) {
            strncpy(root_fs[i].name, filename, MAX_FILENAME - 1);
            root_fs[i].name[MAX_FILENAME - 1] = '\0';
            strncpy(root_fs[i].content, content, 1024 - 1);
            root_fs[i].content[1023] = '\0';
            root_fs[i].size = strlen(content);
            root_fs[i].active = 1;
            printk(LOG_INFO, "VFS: Fichier cree: %s", filename);
            return;
        }
    }
    printk(LOG_ERR, "VFS: Filesystem full, cannot create %s.", filename);
}

void vfs_delete_file(const char* filename) {
    if (!filename) return;
    for(int i = 0; i < MAX_FILES; i++) {
        if(root_fs[i].active && strcmp(root_fs[i].name, filename) == 0) {
            root_fs[i].active = 0;
            printk(LOG_INFO, "VFS: Fichier supprime: %s", filename);
            return;
        }
    }
    printk(LOG_WARN, "VFS: File not found: %s", filename);
}

// Count active files for statistics
int vfs_count_active_files() {
    int count = 0;
    for(int i = 0; i < MAX_FILES; i++) {
        if(root_fs[i].active) count++;
    }
    return count;
}

char* vfs_read_file(const char* filename) {
    if (!filename) return 0;
    
    /* Check cache first */
    if (cache.cached_index >= 0 && strcmp(cache.cached_name, filename) == 0) {
        if (root_fs[cache.cached_index].active) {
            return root_fs[cache.cached_index].content;  /* Cache hit */
        }
    }
    
    /* Cache miss - search */
    for(int i = 0; i < MAX_FILES; i++) {
        if(root_fs[i].active && strcmp(root_fs[i].name, filename) == 0) {
            /* Update cache */
            strcpy(cache.cached_name, filename);
            cache.cached_index = i;
            return root_fs[i].content;
        }
    }
    return 0; // NULL si non trouvé
}

/* Copy file from src to dst */
int vfs_copy_file(const char* src, const char* dst) {
    if (!src || !dst) return 0;
    
    char* content = vfs_read_file(src);
    if (!content) {
        printk(LOG_WARN, "VFS: Cannot copy, file not found: %s", src);
        return 0;
    }
    
    vfs_create_file(dst, content);
    printk(LOG_INFO, "VFS: File copied: %s -> %s", src, dst);
    return 1;
}

/* Rename file from src to dst */
int vfs_rename_file(const char* src, const char* dst) {
    if (!src || !dst) return 0;
    
    for(int i = 0; i < MAX_FILES; i++) {
        if(root_fs[i].active && strcmp(root_fs[i].name, src) == 0) {
            strncpy(root_fs[i].name, dst, MAX_FILENAME - 1);
            root_fs[i].name[MAX_FILENAME - 1] = '\0';
            printk(LOG_INFO, "VFS: File renamed: %s -> %s", src, dst);
            return 1;
        }
    }
    printk(LOG_WARN, "VFS: Cannot rename, file not found: %s", src);
    return 0;
}

/* Hexdump file contents */
void vfs_hexdump_file(const char* filename) {
    if (!filename) return;
    
    char* content = vfs_read_file(filename);
    if (!content) {
        printk(LOG_WARN, "VFS: File not found: %s", filename);
        return;
    }
    
    kprint("Hexdump of ");
    kprint(filename);
    kprint(":\n");
    
    int size = strlen(content);
    for (int i = 0; i < size; i++) {
        if (i % 16 == 0) {
            if (i > 0) kprint("\n");
            char buf[16];
            for (int j = i / 16; j > 0; j /= 16) {}
            printk(LOG_INFO, "%04X: ", i);
        }
        char hex[4];
        unsigned char byte = (unsigned char)content[i];
        sprintf_internal(hex, "%02X ", byte);
        kprint(hex);
    }
    kprint("\n");
}

/* Get uptime in seconds (placeholder) */
uint32_t vfs_get_uptime(void) {
    /* TODO: Get actual timer ticks */
    return 0;
}