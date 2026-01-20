#include "vfs.h"
#include "vga.h"
#include "kbd.h"
#include "string.h"

void mini_vi(char* filename) {
    char new_content[1024];
    int i = 0;
    
    // Initialisation du buffer
    for(int j=0; j<1024; j++) new_content[j] = 0;

    vga_clear();
    kprint_color("--- EDITION: ", 0x70); 
    kprint_color(filename, 0x70);
    kprint_color(" --- (ESC pour quitter, ENTREE pour sauver)\n", 0x70);
    
    while (1) {
        char c = kbd_get_char(); // On utilise ta fonction qui attend une touche

        if (c == 27) { // Touche ESC (Scancode ASCII pour Escape)
            break; 
        } else if (c == '\n') {
            // Dans ce mini-vi, on sauve et on quitte sur ENTREE pour simplifier
            new_content[i] = '\0';
            vfs_create_file(filename, new_content);
            break;
        } else if (c == '\b') { // Backspace
            if (i > 0) {
                i--;
                backspace();
            }
        } else {
            if (i < 1023) {
                new_content[i++] = c;
                kprint_char(c);
            }
        }
    }

    vga_clear();
    kprint("Fichier enregistre dans le VFS.\n");
}