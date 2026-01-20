#ifndef KBD_H
#define KBD_H

#include <stdint.h>

char kbd_get_char();
void readline(char* buffer, int max);
void readline_masked(char* buffer, int max);
char scancode_to_ascii(uint8_t scancode); // Ajoute cette ligne

#endif