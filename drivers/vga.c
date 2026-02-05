#include "../include/vga.h"

char* video_memory = (char*) 0xB8000;
int cursor_pos = 0;

void kprint_char_color(char c, unsigned char color) {
    if (c == '\n') {
        newline();
        return;
    }
    if (cursor_pos < 80 * 25) {
        video_memory[cursor_pos * 2] = c;
        video_memory[cursor_pos * 2 + 1] = color;
        cursor_pos++;
        scroll_screen();
    }
}

void kprint_char(char c) {
    kprint_char_color(c, 0x0F); // Utilise le blanc par défaut
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

// Scroll screen up by one line and move cursor appropriately
void scroll_screen() {
    if (cursor_pos >= 80 * 25) {
        // Shift all lines up by one
        for (int i = 0; i < 80 * 24; i++) {
            video_memory[i * 2]     = video_memory[(i + 80) * 2];
            video_memory[i * 2 + 1] = video_memory[(i + 80) * 2 + 1];
        }
        // Clear last line
        for (int i = 0; i < 80; i++) {
            video_memory[(24 * 80 + i) * 2]     = ' ';
            video_memory[(24 * 80 + i) * 2 + 1] = 0x07;
        }
        cursor_pos = 24 * 80;
    }
}

void newline() {
    cursor_pos = ((cursor_pos / 80) + 1) * 80;
    if (cursor_pos >= 80 * 25) {
        scroll_screen();
    }
}

void backspace() {
    if (cursor_pos > 0) {
        cursor_pos--;
        video_memory[cursor_pos * 2] = ' ';
        video_memory[cursor_pos * 2 + 1] = 0x07;
    }
}

/* Advanced VGA functions for shell input */

void vga_clear_screen(void) {
    clear_screen();
}

void vga_putc(char c) {
    kprint_char(c);
}

int vga_get_cursor_row(void) {
    return cursor_pos / 80;
}

int vga_get_cursor_col(void) {
    return cursor_pos % 80;
}

void vga_set_cursor(int row, int col) {
    cursor_pos = row * 80 + col;
    if (cursor_pos < 0) cursor_pos = 0;
    if (cursor_pos >= 80 * 25) cursor_pos = 80 * 25 - 1;
}

void vga_write(const char* str, int len) {
    for (int i = 0; i < len && str[i]; i++) {
        kprint_char(str[i]);
    }
}

void vga_draw_cursor(void) {
    /* Draw a visible cursor at current position */
    if (cursor_pos < 80 * 25) {
        /* Draw underscore character with bright white */
        video_memory[cursor_pos * 2] = '_';
        video_memory[cursor_pos * 2 + 1] = VGA_BRIGHT_WHITE;
    }
}