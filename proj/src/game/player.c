#include "player.h"

void player_construct() {
    player.position = (struct point2D){300, 300};
    player.angle = 0;
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

int player_set_position(struct point2D position) {
    if (position.x < 800 && position.y < 600)
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
            player.position.y -= (int) (PLAYER_SPEED * sin(player.angle));
            player.position.x += (int) (PLAYER_SPEED * cos(player.angle));
            break;
        case DOWN:
            player.position.y += (int) (PLAYER_SPEED * sin(player.angle));
            player.position.x -= (int) (PLAYER_SPEED * cos(player.angle));
            break;
        case LEFT:
            player.position.y -= (int) (PLAYER_SPEED * cos(player.angle));
            player.position.x -= (int) (PLAYER_SPEED * sin(player.angle));
            break;
        case RIGHT:
            player.position.y += (int) (PLAYER_SPEED * cos(player.angle));
            player.position.x += (int) (PLAYER_SPEED * sin(player.angle));
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
