#ifndef VFS_H
#define VFS_H
#define MAX_NODES 100
typedef struct {
    char name[32];
    int is_dir;
    int parent_idx;
} vfs_node_t;
void vfs_init();
void vfs_mkdir(char* name);
void vfs_list();
#endif