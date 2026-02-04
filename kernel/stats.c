#include "stats.h"
#include "printk.h"
#include "mem.h"
#include "vfs.h"
#include "config.h"

kernel_stats_t kernel_stats = {0};

void stats_init() {
    kernel_stats.heap_total = KMALLOC_HEAP_SIZE;
    kernel_stats.vfs_files_total = MAX_FILES;
    kernel_stats.heap_used = 0;
    kernel_stats.uptime_ms = 0;
    kernel_stats.timer_ticks = 0;
    kernel_stats.pci_devices = 0;
    kernel_stats.vfs_files_used = 1; // motd.txt is pre-created
}

void stats_update() {
    // Update heap usage (simplified: based on placement pointer)
    // In a real OS, would track allocations
    extern uint32_t placement_address, end;
    kernel_stats.heap_used = (placement_address - (uint32_t)&end);
    
    // Update uptime from timer ticks (assuming 1ms per tick)
    extern uint32_t tick;
    kernel_stats.uptime_ms = tick;
    
    // Update VFS file count
    kernel_stats.vfs_files_used = vfs_count_active_files();
}

kernel_stats_t* stats_get() {
    stats_update();
    return &kernel_stats;
}

void stats_print() {
    kernel_stats_t* s = stats_get();
    
    printk(LOG_INFO, "=== Kernel Statistics ===");
    printk(LOG_INFO, "Uptime: %d ms (%d s)", s->uptime_ms, s->uptime_ms / 1000);
    printk(LOG_INFO, "Heap: %d / %d bytes (%d%%)", 
        s->heap_used, s->heap_total,
        (s->heap_used * 100) / (s->heap_total > 0 ? s->heap_total : 1));
    printk(LOG_INFO, "VFS: %d / %d files", s->vfs_files_used, s->vfs_files_total);
    printk(LOG_INFO, "Timer Ticks: %d", s->timer_ticks);
    printk(LOG_INFO, "PCI Devices: %d", s->pci_devices);
}
