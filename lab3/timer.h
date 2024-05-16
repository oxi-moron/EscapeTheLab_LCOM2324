//
// Created by ricardo on 3/14/24.
//

#ifndef G6_TIMER_H
#define G6_TIMER_H

#include <lcom/lcf.h>

int (timer_subscribe_int)(uint8_t *bit_no);
int (timer_unsubscribe_int)();
void (timer_int_handler)();

#endif //G6_TIMER_H
