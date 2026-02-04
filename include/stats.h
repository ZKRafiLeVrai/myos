#ifndef STATS_H
#define STATS_H

#include <stdint.h>

// Kernel statistics structure
typedef struct {
    uint32_t heap_used;         // Bytes allocated from kmalloc
    uint32_t heap_total;        // Total heap size
    uint32_t uptime_ms;         // Uptime in milliseconds
    uint32_t timer_ticks;       // Number of timer interrupts
    uint32_t pci_devices;       // Number of PCI devices found
    uint32_t vfs_files_used;    // Number of active files in VFS
    uint32_t vfs_files_total;   // Maximum files in VFS
} kernel_stats_t;

// Global kernel stats
extern kernel_stats_t kernel_stats;

// Functions
void stats_init();
void stats_update();
kernel_stats_t* stats_get();
void stats_print();

#endif
