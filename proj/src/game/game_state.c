#include "game_state.h"

bool game_in_progress = true;

int game_setup() {
    player_construct();
    if (graphics_construct() != 0) {
        printf("ERROR: %s\n", __func__ );
        return -1;
    }
    if (map_load(map_no) != 0) {
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
        case PAUSE:
            if (graphics_draw_pause_menu() != 0) {
                printf("ERROR: %s\n", __func__ );
                return -1;
            }
            break;
        case INTERCOM:
            if (graphics_draw_intercom() != 0) {
                printf("ERROR: %s\n", __func__ );
                return -1;
            }
            break;
        case VICTORY:
            if (graphics_draw_victory_screen() != 0) {
                printf("ERROR: %s\n", __func__ );
                return -1;
            }
            break;
        case DEFEAT:
            if (graphics_draw_defeat_screen() != 0) {
                printf("ERROR: %s\n", __func__ );
                return -1;
            }
            break;
        case PLAYER_SELECT:
            if (graphics_draw_player_select() != 0) {
                printf("ERROR: %s\n", __func__ );
                return -1;
            }
    }
    return 0;
}

int state_kbd_event(uint8_t scancode) {
    switch (game_state) {
        case GAME:
            if (scancode == W_SCANCODE) {
                if (player_move(UP) != 0) {
                    printf("ERROR: %s\n", __func__ );
                    return -1;
                }
            }
            else if (scancode == A_SCANCODE) {
                if (player_move(LEFT) != 0) {
                    printf("ERROR: %s\n", __func__ );
                    return -1;
                }
            }
            else if (scancode == S_SCANCODE) {
                if (player_move(DOWN) != 0) {
                    printf("ERROR: %s\n", __func__ );
                    return -1;
                }
            }
            else if (scancode == D_SCANCODE) {
                if (player_move(RIGHT) != 0) {
                    printf("ERROR: %s\n", __func__ );
                    return -1;
                }
            }
            else if (scancode == ESC_SCANCODE) {
                game_state = PAUSE;
                if (clock_stop_timer() != 0) {
                    printf("ERROR: %s\n", __func__ );
                    return -1;
                }
            }
            if (player_crossed_door()) {
                map_no += 1;
                if (map_no == FINAL_MAP)
                    game_state = INTERCOM;
                else {
                    map_load(map_no);
                    player_reset_position();
                }
            }
            break;
        case INTERCOM:
            if (intercom_password_entered()) {
                game_state = VICTORY;
            }
            if (scancode == ESC_SCANCODE) {
                game_state = GAME;
                if (clock_stop_timer() != 0) {
                    printf("ERROR: %s\n", __func__ );
                    return -1;
                }
            } else if ((scancode & 0x80) == 0) {
                if (intercom_send_letter(player_no, scancode) != 0) {
                    printf("ERROR: %s\n", __func__ );
                    return -1;
                }
            }
            break;
        case MENU:
            break;
        case PAUSE:
            break;
        case DEFEAT:
            break;
        case VICTORY:
            break;
        case PLAYER_SELECT:
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
            cursor_move(pp.delta_x, pp.delta_y);
            if (pp.lb) {
                uint16_t x, y;
                cursor_get_position(&x, &y);
                if (300 <= x && x <= 520 && y >= 220 && y <= 270) {
                    game_state = PLAYER_SELECT;
                }
                else if (300 <= x && x <= 520 && y >= 330 && y <= 380) game_in_progress = false;
            }
            break;
        case PAUSE:
            cursor_move(pp.delta_x, pp.delta_y);
            if (pp.lb) {
                uint16_t x, y;
                cursor_get_position(&x, &y);
                if (300 <= x && x <= 520 && y >= 220 && y <= 270) {
                    if (clock_start_timer() != 0) {
                        printf("ERROR: %s\n", __func__ );
                        return -1;
                    }
                    game_state = GAME;
                }
                else if (300 <= x && x <= 520 && y >= 330 && y <= 380) {
                    if (clock_reset_timer() != 0) {
                        printf("ERROR: %s\n", __func__ );
                        return -1;
                    }
                    map_no = 0;
                    if (map_load(map_no) != 0) {
                        printf("ERROR: %s\n", __func__);
                        return 1;
                    }
                    player_reset_position();
                    intercom_reset();
                    game_state = MENU;
                }
            }
            break;
        case INTERCOM:
            break;
        case DEFEAT:
            cursor_move(pp.delta_x, pp.delta_y);
            if (pp.lb) {
                uint16_t x, y;
                cursor_get_position(&x, &y);
                if (300 <= x && x <= 520 && y >= 220 && y <= 270) {
                    if (clock_reset_timer() != 0) {
                        printf("ERROR: %s\n", __func__ );
                        return -1;
                    }
                    if (clock_start_timer() != 0) {
                        printf("ERROR: %s\n", __func__ );
                        return -1;
                    }
                    map_no = START_MAP;
                    if (map_load(map_no) != 0) {
                        printf("ERROR: %s\n", __func__);
                        return 1;
                    }
                    player_reset_position();
                    intercom_reset();
                    game_state = PLAYER_SELECT;
                }
                else if (300 <= x && x <= 520 && y >= 330 && y <= 380) {
                    if (clock_reset_timer() != 0) {
                        printf("ERROR: %s\n", __func__ );
                        return -1;
                    }
                    map_no = START_MAP;
                    if (map_load(map_no) != 0) {
                        printf("ERROR: %s\n", __func__);
                        return 1;
                    }
                    player_reset_position();
                    game_state = MENU;
                }
            }
        case VICTORY:
            cursor_move(pp.delta_x, pp.delta_y);
            if (pp.lb) {
                uint16_t x, y;
                cursor_get_position(&x, &y);
                if (300 <= x && x <= 520 && y >= 220 && y <= 270) {
                    if (clock_reset_timer() != 0) {
                        printf("ERROR: %s\n", __func__ );
                        return -1;
                    }
                    if (clock_start_timer() != 0) {
                        printf("ERROR: %s\n", __func__ );
                        return -1;
                    }
                    map_no = START_MAP;
                    if (map_load(map_no) != 0) {
                        printf("ERROR: %s\n", __func__);
                        return 1;
                    }
                    player_reset_position();
                    intercom_reset();
                    game_state = PLAYER_SELECT;
                }
                else if (300 <= x && x <= 520 && y >= 330 && y <= 380) {
                    if (clock_reset_timer() != 0) {
                        printf("ERROR: %s\n", __func__ );
                        return -1;
                    }
                    map_no = START_MAP;
                    if (map_load(map_no) != 0) {
                        printf("ERROR: %s\n", __func__);
                        return 1;
                    }
                    player_reset_position();
                    game_state = MENU;
                }
            }
            break;
        case PLAYER_SELECT:
            cursor_move(pp.delta_x, pp.delta_y);
            if (pp.lb) {
                uint16_t x, y;
                cursor_get_position(&x, &y);
                if (x < 400) player_no = 1;
                else player_no = 2;
                if (clock_start_timer() != 0) {
                    printf("ERROR: %s\n", __func__ );
                    return -1;
                }
                game_state = GAME;
            }
    }

    return 0;
}

int (state_rtc_event) () {
    if (game_state == GAME) {
        if (clock_reset_timer() != 0) {
            printf("ERROR: %s\n", __func__ );
            return -1;
        }
        game_state = DEFEAT;
    }

    return 0;
}
