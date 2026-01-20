#include "printk.h"
#include "vga.h"
#include "string.h"
#include <stdarg.h>

#define LOG_BUFFER_SIZE 8192
char dmesg_stack[LOG_BUFFER_SIZE];
uint32_t dmesg_ptr = 0;

void itoa_hex(uint32_t n, char* str) {
    char* chars = "0123456789ABCDEF";
    str[0] = '0'; str[1] = 'x';
    for (int i = 7; i >= 0; i--) {
        str[i+2] = chars[n & 0xF];
        n >>= 4;
    }
    str[10] = '\0';
}

void printk(int level, const char* format, ...) {
    va_list args;
    va_start(args, format);

    if (level == LOG_ERR) kprint_color("[ ERROR ] ", 0x0C);
    else if (level == LOG_WARN) kprint_color("[  WARN ] ", 0x0E);
    else kprint_color("[  INFO ] ", 0x0A);

    for (const char* p = format; *p != '\0'; p++) {
        if (*p == '%' && *(p+1) != '\0') {
            p++;
            if (*p == 'x' || *p == 'X') {
                char hex[11];
                itoa_hex(va_arg(args, uint32_t), hex);
                kprint(hex);
                for(int j=0; hex[j]; j++) {
                    if (dmesg_ptr < LOG_BUFFER_SIZE - 1) dmesg_stack[dmesg_ptr++] = hex[j];
                }
            }
        } else {
            kprint_char(*p);
            if (dmesg_ptr < LOG_BUFFER_SIZE - 1) dmesg_stack[dmesg_ptr++] = *p;
        }
    }
    
    va_end(args);
    newline();
    if (dmesg_ptr < LOG_BUFFER_SIZE - 1) dmesg_stack[dmesg_ptr++] = '\n';
    dmesg_stack[dmesg_ptr] = '\0';
}

// CETTE FONCTION MANQUAIT AU LINKER
void sys_dmesg() {
    kprint_color("--- Kernel Log Buffer (dmesg) ---\n", 0x0B);
    kprint(dmesg_stack);
    newline();
}