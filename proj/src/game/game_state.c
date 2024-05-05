#include "game_state.h"
#include "../graphics.h"

int game_setup() {
    player_construct();
    if (graphics_construct() != 0) {
        printf("ERROR: %s\n", __func__ );
        return -1;
    }
    return 0;
}

int state_draw_frame() {
    switch(game_state) {
        case MENU:
            if (graphics_draw_menu() != 0) {
                printf("ERROR: %s\n", __func__ );
                return -1;
            }
            break;
        case GAME:
            if (graphics_draw_game() != 0) {
                printf("ERROR: %s\n", __func__ );
                return -1;
            }
            break;
    }
    return 0;
}

int state_kbd_event(uint8_t scancode) {
    switch (game_state) {
        case GAME:
            if (scancode == 0x11) {
                if (player_move(UP) != 0) {
                    printf("ERROR: %s\n", __func__ );
                    return -1;
                }
            }
            if (scancode == 0x1e) {
                if (player_move(LEFT) != 0) {
                    printf("ERROR: %s\n", __func__ );
                    return -1;
                }
            }
            if (scancode == 0x1f) {
                if (player_move(DOWN) != 0) {
                    printf("ERROR: %s\n", __func__ );
                    return -1;
                }
            }
            if (scancode == 0x20) {
                if (player_move(RIGHT) != 0) {
                    printf("ERROR: %s\n", __func__ );
                    return -1;
                }
            }
            break;
        case MENU:
            break;
    }

    return 0;
}

int state_mouse_event(struct packet pp) {
    switch(game_state) {
        case GAME:
            if (pp.delta_x > 1) {
                if (player_move(ROTATE_RIGHT) != 0) {
                    printf("ERROR: %s\n", __func__ );
                    return -1;
                }
            }
            if (pp.delta_x < -1) {
                if (player_move(ROTATE_LEFT) != 0) {
                    printf("ERROR: %s\n", __func__ );
                    return -1;
                }
            }
            break;
        case MENU:
            break;
    }

    return 0;
}
