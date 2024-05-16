#ifndef G6_CURSOR_H
#define G6_CURSOR_H

#include <lcom/lcf.h>

static uint16_t x = 400, y = 300;

void cursor_move(int x_delta, int y_delta);
void cursor_get_position(uint16_t* x_pos, uint16_t* y_pos);

#endif
