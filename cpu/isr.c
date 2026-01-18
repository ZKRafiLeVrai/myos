#include "idt.h"
#include "vga.h"

char *exception_messages[] = {
    "Division By Zero", "Debug", "NMI", "Breakpoint",
    "Overflow", "Bounds", "Invalid Opcode", "No Coprocessor",
    "Double Fault", "Coprocessor Segment Overrun", "Bad TSS", 
    "Segment Not Present", "Stack Fault", "GPF", "Page Fault", "Unknown"
};

void isr_handler(struct registers regs) {
    kprint_color("\n[ KERNEL PANIC ]\n", 0x0C);
    kprint("Exception: ");
    kprint(exception_messages[regs.int_no]);
    for(;;);
}