#ifndef CONFIG_H
#define CONFIG_H

// Kernel configuration and limits

// Memory configuration
#define KMALLOC_HEAP_SIZE   (4 * 1024 * 1024)  // 4 MB kernel heap
#define KMALLOC_MIN_SIZE    1                   // Minimum allocation size
#define KMALLOC_MAX_SIZE    (1024 * 1024)       // 1 MB max single alloc

// VFS configuration
#define VFS_MAX_FILES       32                  // Maximum files in RAM
#define VFS_MAX_FILENAME    64                  // Max filename length
#define VFS_MAX_FILESIZE    1024                // Max file content size

// VGA configuration
#define VGA_WIDTH           80                  // Columns
#define VGA_HEIGHT          25                  // Rows
#define VGA_MEMORY          0xB8000             // Video memory base

// Timer configuration
#define TIMER_FREQUENCY     1000                // 1000 Hz (1 ms per tick)
#define PIT_BASE_FREQ       1193180             // PIT base frequency (Hz)

// Kernel version and branding
#define KERNEL_VERSION_MAJOR    1
#define KERNEL_VERSION_MINOR    0
#define KERNEL_VERSION_PATCH    0
#define KERNEL_CODENAME         "PRO"
#define KERNEL_ARCH             "i386"
#define KERNEL_MODE             "Protected 32-bit"

#endif
