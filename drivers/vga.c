#include "../include/vga.h"

char* video_memory = (char*) 0xB8000;
int cursor_pos = 0;

// Cette fonction manquait ou était mal nommée
void kprint_char(char c) {
    kprint_char_color(c, 0x0F); // Utilise le blanc par défaut
}

void kprint_char_color(char c, unsigned char color) {
    if (c == '\n') {
        newline();
        return;
    }
    video_memory[cursor_pos * 2] = c;
    video_memory[cursor_pos * 2 + 1] = color;
    cursor_pos++;
}

void kprint(const char* str) {
    for (int i = 0; str[i] != '\0'; i++) kprint_char_color(str[i], 0x0F);
}

void kprint_color(const char* str, unsigned char color) {
    for (int i = 0; str[i] != '\0'; i++) kprint_char_color(str[i], color);
}

void clear_screen() {
    for (int i = 0; i < 80 * 25; i++) {
        video_memory[i * 2] = ' ';
        video_memory[i * 2 + 1] = 0x07;
    }
    cursor_pos = 0;
}

void newline() {
    cursor_pos = ((cursor_pos / 80) + 1) * 80;
}

void backspace() {
    if (cursor_pos > 0) {
        cursor_pos--;
        video_memory[cursor_pos * 2] = ' ';
        video_memory[cursor_pos * 2 + 1] = 0x07;
    }
}