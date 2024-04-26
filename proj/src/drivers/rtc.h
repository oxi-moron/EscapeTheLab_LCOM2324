#ifndef G6_RTC_H
#define G6_RTC_H

#include <lcom/lcf.h>

int (rtc_set_alarm) ();
void (rtc_ih) ();

int (rtc_read_from_reg) (uint32_t reg, uint8_t* data);
int (rtc_write_to_reg) (uint32_t reg, uint32_t data);

int (rtc_subscribe_int) (uint8_t* bit_no);
int (rtc_unsubscribe_int) ();

#endif //G6_RTC_H
