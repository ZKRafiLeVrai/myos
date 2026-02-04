#include "idt.h"
#include "vga.h"
#include "printk.h"

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
    newline();

    printk(LOG_INFO, "EAX:%x EBX:%x ECX:%x EDX:%x", regs.eax, regs.ebx, regs.ecx, regs.edx);
    printk(LOG_INFO, "EDI:%x ESI:%x EBP:%x ESP:%x", regs.edi, regs.esi, regs.ebp, regs.esp);
    printk(LOG_INFO, "EIP:%x CS:%x EFLAGS:%x INT:%x ERR:%x", regs.eip, regs.cs, regs.eflags, regs.int_no, regs.err_code);

    __asm__ __volatile__("cli");
    for (;;) __asm__ __volatile__("hlt");
}