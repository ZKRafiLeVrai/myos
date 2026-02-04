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
} inode_t;

// Prototypes des fonctions
void vfs_init();
void sys_ls();
char* vfs_read_file(const char* filename);
void vfs_create_file(const char* filename, const char* content);
void vfs_delete_file(const char* filename);
int vfs_count_active_files();

#endif