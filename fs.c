#include "fs.h"
#include "vga.h"

File files[MAX_FILES];

void fs_init() {
    for(int i = 0; i < MAX_FILES; i++) files[i].exists = 0;
}

int fs_create(const char* name) {
    for(int i = 0; i < MAX_FILES; i++) {
        if(!files[i].exists) {
            for(int j = 0; name[j] != '\0'; j++) files[i].name[j] = name[j];
            files[i].exists = 1;
            return 1;
        }
    }
    return 0;
}

void fs_list() {
    for(int i = 0; i < MAX_FILES; i++) {
        if(files[i].exists) {
            kprint(files[i].name);
            kprint("  ");
        }
    }
}