#include "kbd.h"
#include "io.h"
#include "vga.h"

/* Table de conversion Scancode vers ASCII (Layout QWERTY) */
/* Pour passer en AZERTY, change le 'q' par 'a', le 'w' par 'z', etc. */
unsigned char kbd_map[128] = {
    0,  27, '1', '2', '3', '4', '5', '6', '7', '8', '9', '0', '-', '=', '\b',
    '\t', 'q', 'w', 'e', 'r', 't', 'y', 'u', 'i', 'o', 'p', '[', ']', '\n',
    0, 'a', 's', 'd', 'f', 'g', 'h', 'j', 'k', 'l', ';', '\'', '`', 0, '\\',
    'z', 'x', 'c', 'v', 'b', 'n', 'm', ',', '.', '/', 0, '*', 0, ' ', 0
};

/* Traduit un scancode en caractère ASCII */
char scancode_to_ascii(uint8_t scancode) {
    if (scancode & 0x80) return 0; // Touche relâchée
    return kbd_map[scancode];
}

/* Attend et retourne un caractère frappé au clavier */
char kbd_get_char() {
    char c = 0;
    while (c == 0) {
        // On vérifie si le buffer du clavier (port 0x64) a une donnée (bit 0)
        if (inb(0x64) & 1) {
            uint8_t scancode = inb(0x60);
            c = scancode_to_ascii(scancode);
        }
    }
    return c;
}

/* Lit une ligne entière (pour le shell ou login) */
void readline(char* buffer, int max) {
    int i = 0;
    while (i < max - 1) {
        char c = kbd_get_char();
        
        if (c == '\n') {
            break;
        } else if (c == '\b') { // Backspace
            if (i > 0) {
                i--;
                backspace();
            }
        } else if (c >= 32 && c <= 126) {
            buffer[i++] = c;
            kprint_char(c);
        }
    }
    buffer[i] = '\0';
    kprint_char('\n');
}

/* Lit une ligne en affichant des étoiles (pour le mot de passe) */
void readline_masked(char* buffer, int max) {
    int i = 0;
    while (i < max - 1) {
        char c = kbd_get_char();
        
        if (c == '\n') {
            break;
        } else if (c == '\b') {
            if (i > 0) {
                i--;
                backspace();
            }
        } else if (c >= 32 && c <= 126) {
            buffer[i++] = c;
            kprint_char('*');
        }
    }
    buffer[i] = '\0';
    kprint_char('\n');
}