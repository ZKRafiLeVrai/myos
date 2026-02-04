#ifndef VGA_H
#define VGA_H

void kprint(const char* str);
void kprint_color(const char* str, unsigned char color);
void kprint_char(char c);
void kprint_char_color(char c, unsigned char color);
void clear_screen();
void vga_clear_screen(void);
void newline();
void backspace();
void scroll_screen();
void vga_putc(char c);
void vga_write(const char* str, int len);
int vga_get_cursor_row(void);
int vga_get_cursor_col(void);
void vga_set_cursor(int row, int col);

#endif