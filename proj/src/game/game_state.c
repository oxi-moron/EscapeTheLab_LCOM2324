#include "game_state.h"
#include "../graphics.h"

int state_process() {
    if (state_draw_frame() != 0) {
        printf("ERROR: %s\n", __func__ );
        return -1;
    }

    // for testing purposes...
    game_state = GAME;
    if (game_state == GAME) {
        state_move_player();
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

int state_move_player() {
    if (player_move(ROTATE_LEFT) != 0) {
        printf("ERROR: %s\n", __func__ );
        vg_exit();
        return 1;
    }
    return 0;
}
