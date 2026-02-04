#ifndef MEM_H
#define MEM_H
#include <stdint.h>

void* kmalloc(uint32_t size);
void* kmalloc_a(uint32_t size, uint32_t align);

#endif