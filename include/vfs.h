#ifndef VFS_H
#define VFS_H

#include <stdint.h>

#define MAX_FILES 32
#define MAX_FILENAME 64

// Structure d'un fichier (Inode simplifié)
typedef struct {
    char name[MAX_FILENAME];
    uint32_t size;
    char content[1024]; // Taille max d'un petit fichier en RAM
    uint8_t active;     // 1 si le fichier existe, 0 sinon
    uint32_t ctime;     // Creation time (secondes depuis boot)
    uint32_t mtime;     // Modification time (secondes depuis boot)
} inode_t;

// Prototypes des fonctions
void vfs_init();
void sys_ls();
char* vfs_read_file(const char* filename);
void vfs_create_file(const char* filename, const char* content);
void vfs_delete_file(const char* filename);
int vfs_count_active_files();
int vfs_copy_file(const char* src, const char* dst);
int vfs_rename_file(const char* src, const char* dst);
void vfs_hexdump_file(const char* filename);
uint32_t vfs_get_uptime();  /* Get seconds since boot */

#endif