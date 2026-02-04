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

/* Extended key codes for special keys (with Ctrl, Alt, etc.) */
static int alt_active = 0;
static int ctrl_active = 0;
static int num_lock = 1;
static int extended_key = 0;

unsigned char kbd_read_scancode() {
    if (inb(0x64) & 1) {
        return inb(0x60);
    }
    return 0;
}

char scancode_to_ascii(unsigned char sc) {
    // Handle extended key prefix (0xE0)
    if (sc == 0xE0) {
        extended_key = 1;
        return 0;
    }
    
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
    
    // Gestion de CTRL (Pressé)
    if (sc == 0x1D && !extended_key) {
        ctrl_active = 1;
        return 0;
    }
    
    // Gestion de CTRL (Relâché)
    if ((sc == 0x9D || sc == 0x1D) && extended_key) {
        ctrl_active = 0;
        extended_key = 0;
        return 0;
    }
    
    // Gestion d'ALT (Pressé)
    if (sc == 0x38) {
        alt_active = 1;
        return 0;
    }
    
    // Gestion d'ALT (Relâché)
    if (sc == 0xB8) {
        alt_active = 0;
        return 0;
    }
    
    // Gestion du CAPS LOCK
    if (sc == 0x3A) { 
        caps_lock = !caps_lock; 
        return 0; 
    }
    
    // Gestion de NUM LOCK
    if (sc == 0x45) {
        num_lock = !num_lock;
        return 0;
    }

    // Ignorer le relâchement des touches normales
    if (sc & 0x80) return 0;

    // Touches spéciales (non-printable) avec Ctrl
    if (ctrl_active) {
        switch étendues (flèches, etc.) - précédées par 0xE0
    if (extended_key) {
        extended_key = 0;
        switch (sc) {
            case 0x48: return 0x48;  // UP arrow
            case 0x50: return 0x50;  // DOWN arrow
            case 0x4B: return 0x4B;  // LEFT arrow
            case 0x4D: return 0x4D;  // RIGHT arrow
            case 0x47: return 0x47;  // HOME key
            case 0x4F: return 0x4F;  // END key
            case 0x49: return 0x49;  // PAGE UP
            case 0x51: return 0x51;  // PAGE DOWN
            case 0x52: return 0x52;  // INSERT
            case 0x53: return 0x53;  // DELETE
            default: return 0;
        }
    }
    
    // Touches (sc) {
            case 0x2E: return 3;   // Ctrl+C
            case 0x20: return 4;   // Ctrl+D
            case 0x26: return 12;  // Ctrl+L (clear)
            case 0x16: return 21;  // Ctrl+U (clear line)
            case 0x2D: return 23;  // Ctrl+W (kill word)
            case 0x01: return 27;  // Ctrl+ESC
            default: break;
        }
    }

    // Touches spéciales (non-printable)
    switch (sc) {
        case 0x01: return 27;   // ESC
        case 0x0E: return '\b'; // Backspace
        case 0x0F: return '\t'; // Tab
        case 0x1C: return '\n'; // Enter
        case 0x37: return '*';  // * (numpad)
        case 0x4A: return '-';  // - (numpad)
        case 0x4E: return '+';  // + (numpad)
        default: break;
    }

    // Protection contre les index invalides
    if (sc >= 58) return 0;

    // Retourne le caractère selon l'état
    if (shift_active || caps_lock) {
        return azerty_shift_map[sc];
    } else {
        return azerty_map[sc];
    }
}