#ifndef PRINTK_H
#define PRINTK_H

#include <stdint.h>
#include <stdarg.h>

#define LOG_INFO  0
#define LOG_WARN  1
#define LOG_ERR   2

void printk(int level, const char* format, ...);
void sys_dmesg(); // Doit être ici

#endif