#include "vga.h"

static int cursor_x = 0;
static int cursor_y = 0;
static uint8_t* video_mem = (uint8_t*)0xB8000;

void vga_clear() {
    for (int i = 0; i < 80 * 25 * 2; i += 2) {
        video_mem[i] = ' ';
        video_mem[i+1] = 0x07;
    }
    cursor_x = 0;
    cursor_y = 0;
}

void newline() {
    cursor_x = 0;
    cursor_y++;
    if (cursor_y >= 25) {
        vga_clear(); // Simple pour l'instant
    }
}

void kprint_char(char c) {
    if (c == '\n') {
        newline();
        return;
    }
    int offset = (cursor_y * 80 + cursor_x) * 2;
    video_mem[offset] = c;
    video_mem[offset + 1] = 0x07;
    cursor_x++;
}

void kprint(char* str) {
    for (int i = 0; str[i] != '\0'; i++) {
        kprint_char(str[i]);
    }
}

void kprint_color(char* str, uint8_t color) {
    for (int i = 0; str[i] != '\0'; i++) {
        int offset = (cursor_y * 80 + cursor_x) * 2;
        video_mem[offset] = str[i];
        video_mem[offset + 1] = color;
        cursor_x++;
    }
}

void backspace() {
    if (cursor_x > 0) {
        cursor_x--;
        int offset = (cursor_y * 80 + cursor_x) * 2;
        video_mem[offset] = ' ';
    }
}