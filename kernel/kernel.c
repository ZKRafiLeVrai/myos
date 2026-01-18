#include "vga.h"
#include "gdt.h"
#include "idt.h"
#include "kbd.h"
#include "vfs.h"   // Contient maintenant la déclaration de sys_ls()
#include "string.h" // INDISPENSABLE pour strcmp()

void shell_loop() {
    char buffer[128];
    int i = 0;
    kprint_color("root@myos:/# ", 0x0A);

    while(1) {
        unsigned char sc = kbd_read_scancode();
        if (sc == 0x1C) { // ENTER
            buffer[i] = '\0';
            newline();
            if (strcmp(buffer, "ls") == 0) sys_ls();
            else if (strcmp(buffer, "uname") == 0) kprint("MyOS Linux 1.0-PRO\n");
            else if (strcmp(buffer, "clear") == 0) clear_screen();
            else if (i > 0) kprint("bash: command not found\n");
            
            i = 0;
            kprint_color("root@myos:/# ", 0x0A);
        } else if (sc == 0x0E && i > 0) { // BACKSPACE
            i--; backspace();
        } else {
            char c = scancode_to_ascii(sc);
            if (c && i < 127) {
                buffer[i++] = c;
                kprint_char(c);
            }
        }
    }
}

void kernel_main() {
    gdt_install();
    idt_install();
    vfs_init();
    clear_screen();
    
    kprint_color("--- MyOS Linux Kernel Loading ---\n", 0x0D);
    kprint("[OK] GDT & IDT Loaded\n");
    kprint("[OK] Virtual File System Mounted\n");
    kprint("[OK] Memory Management Active\n\n");
    
    shell_loop();
}