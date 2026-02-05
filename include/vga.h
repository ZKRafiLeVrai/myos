#ifndef VGA_H
#define VGA_H

/* VGA Colors (background | foreground) */
#define VGA_BLACK       0x00
#define VGA_BLUE        0x01
#define VGA_GREEN       0x02
#define VGA_CYAN        0x03
#define VGA_RED         0x04
#define VGA_MAGENTA     0x05
#define VGA_YELLOW      0x06
#define VGA_WHITE       0x07
#define VGA_GRAY        0x08
#define VGA_LIGHT_BLUE  0x09
#define VGA_LIGHT_GREEN 0x0A
#define VGA_LIGHT_CYAN  0x0B
#define VGA_LIGHT_RED   0x0C
#define VGA_LIGHT_MAGENTA 0x0D
#define VGA_LIGHT_YELLOW  0x0E
#define VGA_BRIGHT_WHITE  0x0F

/* ANSI Escape Sequences (simplified) */
#define ANSI_RED        "\x1B[31m"
#define ANSI_GREEN      "\x1B[32m"
#define ANSI_YELLOW     "\x1B[33m"
#define ANSI_BLUE       "\x1B[34m"
#define ANSI_MAGENTA    "\x1B[35m"
#define ANSI_CYAN       "\x1B[36m"
#define ANSI_WHITE      "\x1B[37m"
#define ANSI_RESET      "\x1B[0m"
#define ANSI_BOLD       "\x1B[1m"

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
void vga_draw_cursor(void);  /* Draw visible cursor */

#endif