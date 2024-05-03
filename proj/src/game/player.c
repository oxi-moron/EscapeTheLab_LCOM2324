#include "player.h"

void player_construct(struct point2D position, double angle) {
    player.position = position;
    player.angle = angle;
    for (int i = 0; i < 4; i++) {
        player.player_items[i] = ITEM1;
    }
}

struct point2D player_get_position() {
    return player.position;
}

double player_get_angle() {
    return player.angle;
}

// TODO: Sanitize inputs
int player_set_position(struct point2D position) {
    player.position = position;

    return 0;
}

int player_set_angle(double angle) {
    player.angle = angle;

    return 0;
}

enum items* player_get_items() {
    return player.player_items;
}

int player_move(enum player_moves move) {
    switch(move) {
        case UP:
            player.position.y += PLAYER_SPEED;
            break;
        case DOWN:
            player.position.y -= PLAYER_SPEED;
            break;
        case LEFT:
            player.position.x -= PLAYER_SPEED;
            break;
        case RIGHT:
            player.position.x += PLAYER_SPEED;
            break;
        case ROTATE_LEFT:
            player.angle = (int)(player.angle + PLAYER_SPEED) % 360;
            break;
        case ROTATE_RIGHT:
            player.angle = player.angle - PLAYER_SPEED;
            if (player.angle < 0) player.angle += 360;
            break;
    }
    return 0;
}
