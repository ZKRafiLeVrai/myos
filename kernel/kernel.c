#include "vga.h"
#include "gdt.h"
#include "idt.h"
#include "kbd.h"
#include "vfs.h"
#include "string.h"
#include "printk.h"
#include "pci.h"
#include "io.h"
#include "rtc.h"
#include "stats.h"

void handle_command(char* input) {
    int len = strlen(input);
    while (len > 0 && (input[len-1] == ' ' || input[len-1] == '\n')) {
        input[--len] = '\0';
    }

    if (strcmp(input, "ls") == 0) sys_ls();
    else if (strcmp(input, "dmesg") == 0) sys_dmesg();
    else if (strcmp(input, "pci") == 0) pci_scan();
    else if (strcmp(input, "clear") == 0) clear_screen();
   else if (strcmp(input, "reboot") == 0) {
        kprint("System rebooting...\n");
        __asm__ __volatile__("cli; hlt");
    }
    else if (strcmp(input, "uname") == 0) {
        kprint("MyOS Linux 1.0.0-PRO (AZERTY)\n");
        kprint("Arch: i386 | Mode: Protected 32-bit\n");
    }
   else if (strcmp(input, "touch") == 0) {
        vfs_create_file("nouveau.txt", "Ceci est un nouveau fichier.\n");
        kprint("Fichier 'nouveau.txt' cree. Tapez 'ls' pour voir.\n");
    }
    else if (strcmp(input, "help") == 0) {
        kprint("=== MyOS Commands ===\n");
        kprint("ls      - List files in root directory\n");
        kprint("cat     - Read file 'motd.txt'\n");
        kprint("touch   - Create 'nouveau.txt'\n");
        kprint("dmesg   - Show kernel log buffer\n");
        kprint("pci     - Scan PCI devices\n");
        kprint("date    - Show current time\n");
        kprint("stats   - Show kernel statistics\n");
        kprint("uname   - Show system info\n");
        kprint("clear   - Clear screen\n");
        kprint("reboot  - Reboot system\n");
        kprint("help    - Show this help\n");
    }
   else if (strcmp(input, "cat") == 0) {
        // Pour l'instant, on lit un fichier de test
        char* content = vfs_read_file("motd.txt");
        if (content) kprint(content);
        else kprint("Erreur: Fichier introuvable.\n");
    }
    else if (strcmp(input, "date") == 0) {
        show_time();
    }
    else if (strcmp(input, "uptime") == 0) {
        kprint("Kernel uptime: Please implement timer integration.\n");
    }
    else if (strcmp(input, "stats") == 0) {
        stats_print();
    }
    else if (len > 0) {
        kprint("bash: "); kprint(input); kprint(": command not found\n");
    }
}

void shell_loop() {
    char buffer[128];
    int i = 0;
    kprint_color("root@myos:/# ", 0x0A);

    while(1) {
        unsigned char sc = kbd_read_scancode();
        if (sc == 0) continue;
        if (sc & 0x80) { scancode_to_ascii(sc); continue; }

        if (sc == 0x1C) {
            buffer[i] = '\0';
            newline();
            handle_command(buffer);
            i = 0;
            kprint_color("root@myos:/# ", 0x0A);
        } else if (sc == 0x0E && i > 0) {
            i--; backspace();
        } else {
            char c = scancode_to_ascii(sc);
            if (c && i < 127) { buffer[i++] = c; kprint_char(c); }
        }
        for(volatile int p = 0; p < 20000; p++); 
    }
}

void kernel_main() {
    clear_screen();
    gdt_install();
    idt_install();
    vfs_init();
    stats_init();
    
    printk(LOG_INFO, "Linux version 1.0.0-PRO (gcc 11.4.0) 2026");
    printk(LOG_INFO, "CPU: GDT/IDT Loaded");
    printk(LOG_INFO, "PCI: Scanning devices...");
    pci_scan(); 

    kprint_color("\nWelcome to MyOS (tty1)\n", 0x0B);
    kprint("Keyboard: AZERTY FR Active\n");
    kprint("Type 'help' for available commands.\n\n");
    shell_loop();
}