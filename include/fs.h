#ifndef FS_H
#define FS_H

#define MAX_FILES 16
#define MAX_FILE_NAME 32
#define MAX_CONTENT 256

typedef struct {
    char name[MAX_FILE_NAME];
    char content[MAX_CONTENT];
    int exists;
} File;

void fs_init();
int fs_create(const char* name);
void fs_list();
#endif