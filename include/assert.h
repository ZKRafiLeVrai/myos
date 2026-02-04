#ifndef ASSERT_H
#define ASSERT_H

#include "printk.h"

// Compile-time assertion (static)
#define STATIC_ASSERT(expr) \
    extern char static_assertion[(expr) ? 1 : -1]

// Runtime assertion with kernel panic on failure
#define ASSERT(expr) \
    do { \
        if (!(expr)) { \
            printk(LOG_ERR, "ASSERT FAILED: %s at %s:%d", #expr, __FILE__, __LINE__); \
            __asm__ __volatile__("cli; hlt"); \
        } \
    } while(0)

// Warnings (non-fatal)
#define WARN_ON(expr) \
    do { \
        if ((expr)) { \
            printk(LOG_WARN, "WARN: %s at %s:%d", #expr, __FILE__, __LINE__); \
        } \
    } while(0)

#endif
