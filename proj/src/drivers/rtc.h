#ifndef G6_RTC_H
#define G6_RTC_H

#include <lcom/lcf.h>

int (rtc_set_alarm) (uint8_t alarm_hours, uint8_t alarm_minutes, uint8_t alarm_seconds);
void (rtc_ih) ();
int (rtc_disable_alarm) ();

int (rtc_subscribe_int) (uint8_t* bit_no);
int (rtc_unsubscribe_int) ();

int (rtc_get_time) (uint8_t* hours, uint8_t* minutes, uint8_t* seconds);


int (rtc_read_from_reg) (uint32_t reg, uint8_t* data);
int (rtc_write_to_reg) (uint32_t reg, uint8_t data);

uint8_t (to_binary) (uint8_t bcd);
uint8_t (to_bcd) (uint8_t binary);

#endif //G6_RTC_H
