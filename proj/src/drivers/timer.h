#ifndef G6_TIMER_H
#define G6_TIMER_H

#include <lcom/lcf.h>

int (timer_get_frequency)(uint32_t* freq);
int (timer_set_frequency)(uint8_t timer, uint32_t freq);
int (timer_subscribe_int)(uint8_t* bit_no);
int (timer_unsubscribe_int)();

void (timer_ih) ();

int timer_write_cmd(uint8_t cmd);
int timer_read_status(uint8_t* st, uint8_t timer);

#endif //G6_TIMER_H
