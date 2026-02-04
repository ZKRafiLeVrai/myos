#include "io.h"
#include "printk.h"

#define CMOS_ADDR 0x70
#define CMOS_DATA 0x71

uint8_t get_rtc_register(int reg) {
    outb(CMOS_ADDR, reg);
    return inb(CMOS_DATA);
}

void show_time() {
    uint8_t second = get_rtc_register(0x00);
    uint8_t minute = get_rtc_register(0x02);
    uint8_t hour   = get_rtc_register(0x04);
    
    // Conversion BCD en binaire (le RTC stocke souvent en BCD)
    hour = (hour & 0x0F) + ((hour / 16) * 10);
    minute = (minute & 0x0F) + ((minute / 16) * 10);
    second = (second & 0x0F) + ((second / 16) * 10);

    printk(LOG_INFO, "Time: %x:%x:%x", (uint32_t)hour, (uint32_t)minute, (uint32_t)second);
}