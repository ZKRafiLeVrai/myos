#ifndef ERROR_H
#define ERROR_H

// Standard error codes
typedef enum {
    E_OK        = 0,   // Success
    E_INVAL     = 1,   // Invalid argument
    E_NOMEM     = 2,   // Memory allocation failed
    E_NOENT     = 3,   // File/resource not found
    E_EXIST     = 4,   // File/resource already exists
    E_FULL      = 5,   // Storage is full
    E_IO        = 6,   // I/O error
    E_PERM      = 7,   // Permission denied
    E_AGAIN     = 8,   // Try again (temporary failure)
    E_FAULT     = 9,   // Page/segmentation fault
    E_ACCES     = 10,  // Access denied
} error_t;

// Helper macro to check error and return
#define ERR_CHECK(code) do { if ((code) != E_OK) return (code); } while(0)

#endif
