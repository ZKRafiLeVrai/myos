#ifndef VFS_H
#define VFS_H

#include <stdint.h>

#define MAX_FILES 64

typedef struct {
    char name[32];
    uint32_t size;
    uint32_t inode;
    char content[512];
    uint8_t type; // 1: Dir, 2: File
} inode_t;

void vfs_init();
void sys_ls();

#endif