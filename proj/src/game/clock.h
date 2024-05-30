#ifndef G6_CLOCK_H
#define G6_CLOCK_H

#include <lcom/lcf.h>
#include "../drivers/rtc.h"

static uint8_t remaining_hours = 0, remaining_minutes = 10, remaining_seconds = 0;
static uint8_t start_hours, start_minutes, start_seconds;

int (clock_start_timer) ();
int (clock_stop_timer) ();
int (clock_reset_timer) ();

#endif //G6_CLOCK_H
