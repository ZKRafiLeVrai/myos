#include "pci.h"
#include "io.h"     // Pour outl et inl
#include "printk.h" // Pour printk

uint32_t pci_read_config(uint8_t bus, uint8_t slot, uint8_t func, uint8_t offset) {
    uint32_t address = (uint32_t)((bus << 16) | (slot << 11) | (func << 8) | (offset & 0xfc) | ((uint32_t)0x80000000));
    outl(0xCF8, address);
    return inl(0xCFC);
}

void pci_scan() {
    for(int bus = 0; bus < 256; bus++) {
        for(int slot = 0; slot < 32; slot++) {
            uint32_t vendor = pci_read_config(bus, slot, 0, 0) & 0xFFFF;
            if(vendor != 0xFFFF) {
                uint32_t device = (pci_read_config(bus, slot, 0, 0) >> 16) & 0xFFFF;
                printk(0, "PCI: Device Found - Vendor:0x%X Device:0x%X", (uint32_t)vendor, (uint32_t)device);
            }
        }
    }
}