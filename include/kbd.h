#ifndef KBD_H
#define KBD_H

/* Extended key codes */
#define KEY_UP          200
#define KEY_DOWN        201
#define KEY_LEFT        202
#define KEY_RIGHT       203
#define KEY_HOME        204
#define KEY_END         205
#define KEY_DELETE      206
#define KEY_PGUP        207
#define KEY_PGDN        208

/* Control key codes */
#define CTRL_A          1
#define CTRL_C          3
#define CTRL_D          4
#define CTRL_E          5
#define CTRL_K          11
#define CTRL_L          12
#define CTRL_U          21
#define CTRL_W          23

unsigned char kbd_read_scancode();
char scancode_to_ascii(unsigned char scancode);

extern int extended_key;  /* Flag set by kbd driver when 0xE0 prefix received */

#endif