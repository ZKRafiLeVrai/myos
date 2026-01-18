#include <stdint.h>

#define HEAP_START 0x1000000 // 16MB
uint32_t heap_current = HEAP_START;

void* kmalloc(uint32_t size) {
    uint32_t addr = heap_current;
    heap_current += size;
    // Alignement 4-byte
    if (heap_current % 4) heap_current += (4 - (heap_current % 4));
    return (void*)addr;
}