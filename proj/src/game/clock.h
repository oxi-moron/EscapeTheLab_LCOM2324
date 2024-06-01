#ifndef G6_CLOCK_H
#define G6_CLOCK_H

#include <lcom/lcf.h>
#include "../drivers/rtc.h"
#include "game_macros.h"

static uint8_t remaining_hours = HOUR_LIMIT, remaining_minutes = MINUTE_LIMIT, remaining_seconds = SECOND_LIMIT;
static uint8_t start_hours, start_minutes, start_seconds;

/**
 * @brief Starts the in-game clock timer.
 * @return 0 if successful, non-zero otherwise.
 */
int (clock_start_timer) ();

/**
 * @brief Stops the in-game clock timer.
 * @return 0 if successful, non-zero otherwise.
 */
int (clock_stop_timer) ();

/**
 * @brief Resets the in-game clock timer to 10 minutes.
 * @return 0 if successful, non-zero otherwise.
 */
int (clock_reset_timer) ();

#endif //G6_CLOCK_H
