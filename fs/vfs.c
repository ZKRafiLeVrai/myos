#include "vfs.h"
#include "string.h"
#include "vga.h"
#include "printk.h"

inode_t root_fs[MAX_FILES];

void vfs_init() {
    // Initialisation du système de fichiers à vide
    for(int i = 0; i < MAX_FILES; i++) {
        root_fs[i].active = 0;
    }

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

char* vfs_read_file(const char* filename) {
    for(int i = 0; i < MAX_FILES; i++) {
        if(root_fs[i].active && strcmp(root_fs[i].name, filename) == 0) {
            return root_fs[i].content;
        }
    }
    return 0; // NULL si non trouvé
}