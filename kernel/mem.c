#include <stdint.h>

extern uint32_t end; // Défini par le linker script
uint32_t placement_address = (uint32_t)&end;

// Limit kernel heap to 4 MB (simple protection against runaway allocation)
#define KMALLOC_MAX_SIZE (1024 * 1024 * 4)

// Alignement simple: s'assure que les allocations sont alignées sur 4 octets
void* kmalloc(uint32_t sz) {
    // Sanity check: reject zero or suspiciously large allocations
    if (sz == 0 || sz > KMALLOC_MAX_SIZE) {
        return 0; // NULL
    }
    
    // Check if allocation would exceed heap limit
    if ((placement_address - (uint32_t)&end) + sz > KMALLOC_MAX_SIZE) {
        return 0; // Heap exhausted
    }
    
    if (placement_address & 0x3) {
        placement_address = (placement_address + 3) & ~0x3;
    }
    uint32_t tmp = placement_address;
    placement_address += sz;
    return (void*)tmp;
}

// Allocation avec alignement dynamique (align doit être puissance de deux)
void* kmalloc_a(uint32_t sz, uint32_t align) {
    if (sz == 0 || sz > KMALLOC_MAX_SIZE) return 0;
    if (align == 0) align = 4;
    if ((align & (align - 1)) != 0) align = 4;
    
    if ((placement_address - (uint32_t)&end) + sz > KMALLOC_MAX_SIZE) {
        return 0; // Heap exhausted
    }
    
    if (placement_address & (align - 1)) {
        placement_address = (placement_address + align - 1) & ~(align - 1);
    }
    uint32_t tmp = placement_address;
    placement_address += sz;
    return (void*)tmp;
}