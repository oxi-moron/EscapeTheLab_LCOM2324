//
// Created by ricardo on 3/14/24.
//

#ifndef G6_MOUSE_H
#define G6_MOUSE_H

#include <lcom/lcf.h>
#include "i8042.h"

typedef enum {INIT, DRAW_UP, VERTEX, DRAW_DOWN, DONE} state_t;
typedef enum {R_UP, R_DOWN, L_UP, L_DOWN, MOVE_UP, MOVE_DOWN, ILLEGAL, NEUTRAL} event_t;

int (mouse_subscribe_int) (uint8_t* bit_no);
int (mouse_unsubscribe_int) ();
void (mouse_ih) ();

int (mouse_disable_data_reporting) ();
int (my_mouse_enable_data_reporting) ();

state_t (change_state) (state_t current_state, event_t current_event, uint8_t x_len);
event_t (evaluate_event) (event_t current_event, uint8_t tolerance);
bool (validate_line_up) (uint8_t x_len);
bool (validate_line_down) (uint8_t x_len);

#endif //G6_MOUSE_H
