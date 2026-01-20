#include "vga.h"
#include "gdt.h"
#include "idt.h"
#include "kbd.h"
#include "vfs.h"
#include "string.h"

// Prototypes internes pour éviter les avertissements
void shell_loop();
void handle_command(char* input);

// LA FONCTION QUE LE BOOT.S RECHERCHE
void kernel_main() {
    // 1. Initialisation du matériel de base
    gdt_install();
    idt_install();
    
    // 2. Initialisation des services
    vfs_init();
    clear_screen();
    
    // 3. Message de bienvenue (Linux Style)
    kprint_color("Welcome to MyOS Linux Kernel v1.0\n", 0x0D);
    kprint("[OK] CPU Descriptors Loaded\n");
    kprint("[OK] Virtual File System Mounted\n");
    kprint("[OK] Keyboard Driver Active\n\n");
    
    // 4. Lancement de l'interface utilisateur
    shell_loop();
}

void handle_command(char* input) {
    if (strcmp(input, "ls") == 0) {
        sys_ls();
    } else if (strcmp(input, "clear") == 0) {
        clear_screen();
    } else if (strcmp(input, "help") == 0) {
        kprint("Commands: ls, clear, help, uname, whoami\n");
    } else if (strcmp(input, "uname") == 0) {
        kprint("MyOS Linux 1.0.0-release i386\n");
    } else if (strcmp(input, "whoami") == 0) {
        kprint("root\n");
    } else if (strlen(input) > 0) {
        kprint("bash: ");
        kprint(input);
        kprint(": command not found\n");
    }
}

void shell_loop() {
    char buffer[128];
    int i = 0;
    
    kprint_color("root@myos:/# ", 0x0A); // Prompt vert

    while(1) {
        unsigned char sc = kbd_read_scancode();
        if (sc == 0) continue;

        if (sc == 0x1C) { // Touche ENTREE
            buffer[i] = '\0';
            newline();
            handle_command(buffer);
            i = 0;
            kprint_color("root@myos:/# ", 0x0A);
        } 
        else if (sc == 0x0E) { // Touche BACKSPACE
            if (i > 0) {
                i--;
                backspace();
            }
        } 
        else {
            char c = scancode_to_ascii(sc);
            if (c && i < 127) {
                buffer[i++] = c;
                kprint_char(c);
            }
        }
    }
}