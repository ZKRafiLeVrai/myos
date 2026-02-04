#include <stdint.h>

extern uint32_t end; // Défini par le linker script
uint32_t placement_address = (uint32_t)&end;

// Alignement simple: s'assure que les allocations sont alignées sur 4 octets
void* kmalloc(uint32_t sz) {
    if (placement_address & 0x3) {
        placement_address = (placement_address + 3) & ~0x3;
    }
    uint32_t tmp = placement_address;
    placement_address += sz;
    return (void*)tmp;
}

// Allocation avec alignement dynamique (align doit être puissance de deux)
void* kmalloc_a(uint32_t sz, uint32_t align) {
    if (align == 0) align = 4;
    if ((align & (align - 1)) != 0) align = 4;
    if (placement_address & (align - 1)) {
        placement_address = (placement_address + align - 1) & ~(align - 1);
    }
    uint32_t tmp = placement_address;
    placement_address += sz;
    return (void*)tmp;
}