#ifndef RTC_H
#define RTC_H

#include <stdint.h>

void show_time();
uint8_t get_rtc_register(int reg);

#endif