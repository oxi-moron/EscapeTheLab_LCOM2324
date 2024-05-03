#include "player.h"

void player_construct(struct point2D position, double angle) {
    player.position = position;
    player.angle = angle;
    for (int i = 0; i < 4; i++) {
        player.items[i].id = 0;
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

struct Item* player_get_items() {
    return player.items;
}

// TODO: Replace with macros
int player_move(enum player_moves move) {
    switch(move) {
        case UP:
            player.position.y -= 10;
            break;
        case DOWN:
            player.position.y += 10;
            break;
        case LEFT:
            player.position.x -= 10;
            break;
        case RIGHT:
            player.position.x += 10;
            break;
        case ROTATE_LEFT:
            player.angle = (int)(player.angle + 10) % 360;
            break;
        case ROTATE_RIGHT:
            player.angle = player.angle - 10;
            if (player.angle < 0) player.angle += 360;
            break;
    }
    return 0;
}
