#ifndef VGA_H
#define VGA_H

#include <stdint.h>

void vga_clear();
void kprint(char* str);
void kprint_char(char c);
void kprint_color(char* str, uint8_t color);
void newline();
void backspace();

#endif