#include <stdint.h>
#include "io.h"
#include "vga.h"

uint32_t tick = 0;

void timer_callback() {
    tick++;
}

void init_timer(uint32_t frequency) {
    // Fréquence du PIT (Programmable Interval Timer) : 1.193182 MHz
    uint32_t divisor = 1193180 / frequency;

    // Envoyer la commande de configuration (0x36) au port 0x43
    outb(0x43, 0x36);

    // Diviser la fréquence en deux octets (poids faible, puis poids fort)
    uint8_t low = (uint8_t)(divisor & 0xFF);
    uint8_t high = (uint8_t)((divisor >> 8) & 0xFF);

    outb(0x40, low);
    outb(0x40, high);
}