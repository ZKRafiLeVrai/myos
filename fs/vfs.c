#include "vfs.h"
#include "string.h"
#include "vga.h"

// On ne définit PLUS la struct ici, on utilise celle du .h
inode_t root_fs[MAX_FILES];
int file_count = 0;

void vfs_init() {
    strcpy(root_fs[0].name, "bin");
    root_fs[0].type = 1;
    strcpy(root_fs[1].name, "etc");
    root_fs[1].type = 1;
    strcpy(root_fs[2].name, "version");
    strcpy(root_fs[2].content, "MyOS Linux Kernel v1.0.0-Stable\n");
    root_fs[2].type = 2;
    file_count = 3;
}

void sys_ls() {
    kprint("Type  Name\n");
    for(int i = 0; i < file_count; i++) {
        if(root_fs[i].type == 1) kprint_color("[DIR] ", 0x0B);
        else kprint_color("[FIL] ", 0x07);
        kprint(root_fs[i].name);
        kprint("  ");
    }
    newline();
}