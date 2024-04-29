#ifndef G6_GAME_STATE_H
#define G6_GAME_STATE_H

#include <lcom/lcf.h>

enum game_state_t {MENU, GAME};

static enum game_state_t game_state = MENU;

int state_process();


int state_draw_frame();
int state_move_player();

#endif //G6_GAME_STATE_H
