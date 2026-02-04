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
#include "shell.h"
#include "shell_input.h"

void handle_command(char* input) {
    int len = strlen(input);
    while (len > 0 && (input[len-1] == ' ' || input[len-1] == '\n')) {
        input[--len] = '\0';
    }

    if (len == 0) return;

    // Parse command and arguments
    cmd_args_t args = shell_parse_command(input);
    
    // Try new shell command parser first
    int ret = shell_execute_command(args);
    if (ret != -1) return; // Command handled
    
    // Fallback to old single-command handlers
    const char* cmd = args.argv[0];
    
    if (strcmp(cmd, "ls") == 0) sys_ls();
    else if (strcmp(cmd, "dmesg") == 0) sys_dmesg();
    else if (strcmp(cmd, "pci") == 0) pci_scan();
    else if (strcmp(cmd, "clear") == 0) clear_screen();
   else if (strcmp(cmd, "reboot") == 0) {
        kprint("System rebooting...\n");
        __asm__ __volatile__("cli; hlt");
    }
    else if (strcmp(cmd, "uname") == 0) {
        kprint("MyOS Linux 1.0.0-PRO (AZERTY)\n");
        kprint("Arch: i386 | Mode: Protected 32-bit\n");
    }
   else if (strcmp(cmd, "touch") == 0) {
        vfs_create_file("nouveau.txt", "Ceci est un nouveau fichier.\n");
        kprint("Fichier 'nouveau.txt' cree. Tapez 'ls' pour voir.\n");
    }
   else if (strcmp(cmd, "cat") == 0) {
        char* content = vfs_read_file("motd.txt");
        if (content) kprint(content);
        else kprint("Erreur: Fichier introuvable.\n");
    }
    else if (strcmp(cmd, "date") == 0) {
        show_time();
    }
    else if (strcmp(cmd, "stats") == 0) {
        stats_print();
    }
    else {
        kprint("bash: "); kprint(cmd); kprint(": command not found\n");
    }
}

void shell_loop() {
    shell_input_t input;
    shell_input_init(&input);

    while(1) {
        char* cmd = shell_input_read_line(&input, "root@myos:/# ");
        if (cmd && *cmd != '\0') {
            handle_command(cmd);
        }
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