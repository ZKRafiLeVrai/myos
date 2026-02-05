#ifndef KBD_H
#define KBD_H

unsigned char kbd_read_scancode();
char scancode_to_ascii(unsigned char scancode);

extern int extended_key;  /* Flag set by kbd driver when 0xE0 prefix received */

#endif