#ifndef KBD_H
#define KBD_H

unsigned char kbd_read_scancode();
char scancode_to_ascii(unsigned char scancode);

#endif