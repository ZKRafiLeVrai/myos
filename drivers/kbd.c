#include "kbd.h"
#include "io.h"

/* --- États des touches --- */
static int shift_active = 0;
static int caps_lock = 0;

/* --- Mapping AZERTY (Minuscules) --- */
unsigned char azerty_map[] = {
    0,  27, '1', '2', '3', '4', '5', '6', '7', '8', '9', '0', ')', '=', '\b',
    '\t', 'a', 'z', 'e', 'r', 't', 'y', 'u', 'i', 'o', 'p', '^', '$', '\n',
    0, 'q', 's', 'd', 'f', 'g', 'h', 'j', 'k', 'l', 'm', 0x97, '*', 0,
    '<', 'w', 'x', 'c', 'v', 'b', 'n', ',', ';', ':', '!', 0, '*', 0, ' '
};

/* --- Mapping AZERTY (Majuscules) --- */
unsigned char azerty_shift_map[] = {
    0,  27, '!', '@', '#', '$', '%', '^', '&', '*', '(', ')', '_', '+', '\b',
    '\t', 'A', 'Z', 'E', 'R', 'T', 'Y', 'U', 'I', 'O', 'P', '{', '}', '\n',
    0, 'Q', 'S', 'D', 'F', 'G', 'H', 'J', 'K', 'L', 'M', '%', 0x9C, 0,
    '>', 'W', 'X', 'C', 'V', 'B', 'N', '?', '.', '/', 0x15, 0, '*', 0, ' '
};

unsigned char kbd_read_scancode() {
    if (inb(0x64) & 1) {
        return inb(0x60);
    }
    return 0;
}

char scancode_to_ascii(unsigned char sc) {
    // Gestion du SHIFT (Pressé)
    if (sc == 0x2A || sc == 0x36) { 
        shift_active = 1; 
        return 0; 
    }
    
    // Gestion du SHIFT (Relâché)
    if (sc == 0xAA || sc == 0xB6) { 
        shift_active = 0; 
        return 0; 
    }
    
    // Gestion du CAPS LOCK
    if (sc == 0x3A) { 
        caps_lock = !caps_lock; 
        return 0; 
    }

    // Ignorer le relâchement des touches normales
    if (sc & 0x80) return 0;

    // Protection contre les index invalides
    if (sc >= 58) return 0;

    // Retourne le caractère selon l'état
    if (shift_active || caps_lock) {
        return azerty_shift_map[sc];
    } else {
        return azerty_map[sc];
    }
}