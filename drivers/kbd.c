#include "kbd.h"
#include "io.h"

int shift_active = 0;

// Mapping AZERTY Français
unsigned char azerty_map[] = {
    0,  27, '1', '2', '3', '4', '5', '6', '7', '8', '9', '0', ')', '=', '\b',
    '\t', 'a', 'z', 'e', 'r', 't', 'y', 'u', 'i', 'o', 'p', '^', '$', '\n',
    0, 'q', 's', 'd', 'f', 'g', 'h', 'j', 'k', 'l', 'm', '%', '*', 0,
    '<', 'w', 'x', 'c', 'v', 'b', 'n', ',', ';', ':', '!', 0, '*', 0, ' '
};

unsigned char azerty_shift_map[] = {
    0,  27, '1', '2', '3', '4', '5', '6', '7', '8', '9', '0', '0', '+', '\b',
    '\t', 'A', 'Z', 'E', 'R', 'T', 'Y', 'U', 'I', 'O', 'P', ' ', ' ', '\n',
    0, 'Q', 'S', 'D', 'F', 'G', 'H', 'J', 'K', 'L', 'M', ' ', ' ', 0,
    '>', 'W', 'X', 'C', 'V', 'B', 'N', '?', '.', '/', ' ', 0, '*', 0, ' '
};

unsigned char kbd_read_scancode() {
    if (inb(0x64) & 1) return inb(0x60);
    return 0;
}

char scancode_to_ascii(unsigned char sc) {
    if (sc == 0x2A || sc == 0x36) { shift_active = 1; return 0; } // Shift press
    if (sc == 0xAA || sc == 0xB6) { shift_active = 0; return 0; } // Shift release
    if (sc & 0x80 || sc > 58) return 0; // Release ou hors map

    return shift_active ? azerty_shift_map[sc] : azerty_map[sc];
}