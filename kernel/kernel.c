#include "vga.h"
#include "gdt.h"
#include "idt.h"
#include "kbd.h"
#include "string.h"
#include "vfs.h"

void handle_command(char* input) {
    // Dans handle_command :
if (strcmp(input, "reboot") == 0) {
    sys_reboot();
} else if (strcmp(input, "ls") == 0) {
    } else if (strcmp(input, "help") == 0) {
        kprint("Commandes: ls, vi, whoami, clear, reboot, fetch\n");
    } else if (strcmp(input, "whoami") == 0) {
        kprint("Logged as: root\n");
    } else if (strcmp(input, "clear") == 0) {
        vga_clear();
    } else if (strcmp(input, "vi") == 0) {
        kprint("Nom du fichier: ");
        char fname[32];
        readline(fname, 31);
        mini_vi(fname);
    } else if (strlen(input) > 0) {
        kprint("Commande inconnue. Tapez 'help'\n");
    }
}

void kernel_main() {
    vga_clear();
    gdt_install();
    idt_install();
    vfs_init(); // Initialise tes fichiers par défaut
    
    kprint_color("MyOS Linux Pro HD - Boot Success\n", 0x0A);
    kprint("--------------------------------\n");

    // Système de Login
    char user[32];
    char pass[32];
    int auth = 0;

    while(!auth) {
        kprint("\nLogin: ");
        readline(user, 31);
        kprint("Password: ");
        readline_masked(pass, 31);

        if(strcmp(user, "root") == 0 && strcmp(pass, "1234") == 0) {
            auth = 1;
            kprint_color("\nBienvenue root!\n", 0x0B);
        } else {
            kprint_color("\nAcces refuse.\n", 0x0C);
        }
    }

    // Shell principal
    char buffer[128];
    while (1) {
        kprint_color("root@myos:# ", 0x0E);
        readline(buffer, 127);
        handle_command(buffer);
    }
}