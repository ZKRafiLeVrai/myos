#ifndef VGA_H
#define VGA_H

void kprint(const char* str);
void kprint_color(const char* str, unsigned char color);
void kprint_char(char c); // Doit être ici
void kprint_char_color(char c, unsigned char color);
void clear_screen();
void newline();
void backspace();
void scroll_screen();

#endif