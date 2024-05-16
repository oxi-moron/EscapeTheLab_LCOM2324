#ifndef G6_GAME_STATE_H
#define G6_GAME_STATE_H

#include <lcom/lcf.h>
#include "player.h"
#include "cursor.h"

enum game_state_t {MENU, GAME};

static enum game_state_t game_state = MENU;

int game_setup();

int state_draw_frame();
int state_kbd_event(uint8_t scancode);
int state_mouse_event(struct packet pp);

#endif //G6_GAME_STATE_H
