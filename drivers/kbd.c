#include "kbd.h"
#include "io.h"

// Table AZERTY simplifiée
unsigned char kbd_map[128] = {
    0,  27, '1', '2', '3', '4', '5', '6', '7', '8', '9', '0', '-', '=', '\b',
    '\t', 'a', 'z', 'e', 'r', 't', 'y', 'u', 'i', 'o', 'p', '[', ']', '\n',
    0, 'q', 's', 'd', 'f', 'g', 'h', 'j', 'k', 'l', 'm', '%', '*', 0,
    '\\', 'w', 'x', 'c', 'v', 'b', 'n', ',', ';', ':', '!', 0, '*', 0, ' '
};

unsigned char kbd_read_scancode() {
    if (inb(0x64) & 0x01) return inb(0x60);
    return 0;
}

// LA FONCTION MANQUANTE
char scancode_to_ascii(unsigned char scancode) {
    if (scancode > 127) return 0;
    return kbd_map[scancode];
}