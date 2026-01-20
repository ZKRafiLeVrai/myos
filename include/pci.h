#ifndef PCI_H
#define PCI_H

#include <stdint.h>

void pci_scan();
uint32_t pci_read_config(uint8_t bus, uint8_t slot, uint8_t func, uint8_t offset);

#endif