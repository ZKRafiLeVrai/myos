#include <stdint.h>

extern uint32_t end; // Défini par le linker script
uint32_t placement_address = (uint32_t)&end;

void* kmalloc(uint32_t sz) {
    uint32_t tmp = placement_address;
    placement_address += sz;
    return (void*)tmp;
}