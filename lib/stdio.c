#include "../include/vga.h"

void printf(char* str) {
    kprint(str);
}

void print_logo() {
    kprint_color("  __  __        TOS \n", 0x0D);
    kprint_color(" |  \\/  |_   _ / _ \\ ___ \n", 0x0D);
    kprint_color(" | |\\/| | | | | | | / __| \n", 0x0D);
    kprint_color(" | |  | | |_| | |_| \\__ \\ \n", 0x0D);
    kprint_color(" |_|  |_|\\__, |\\___/|___/ \n", 0x0D);
    kprint_color("         |___/            \n", 0x0D);
}