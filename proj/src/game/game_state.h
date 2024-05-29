#ifndef G6_GAME_STATE_H
#define G6_GAME_STATE_H

#include <lcom/lcf.h>
#include "player.h"
#include "cursor.h"
#include "graphics.h"
#include "clock.h"
#include "intercom.h"

enum game_state_t {MENU, GAME, PAUSE, INTERCOM, DEFEAT, VICTORY};

static enum game_state_t game_state = MENU;
static uint8_t map_no = 0;

int game_setup();

int state_draw_frame();
int state_kbd_event(uint8_t scancode);
int state_mouse_event(struct packet pp);
int (state_rtc_event) ();

#endif //G6_GAME_STATE_H
