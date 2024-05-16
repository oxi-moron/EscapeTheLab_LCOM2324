#ifndef G6_PLAYER_H
#define G6_PLAYER_H

#include <lcom/lcf.h>

#include "graphics.h"
#include "game_macros.h"

enum items {ITEM1, ITEM2, ITEM3, ITEM4};

struct Player {
    struct point2D position;
    double angle;
    enum items player_items[INVENTORY_SIZE];
};

enum player_moves {UP, DOWN, LEFT, RIGHT, ROTATE_LEFT, ROTATE_RIGHT};

static struct Player player;

void player_construct();

struct point2D player_get_position();
double player_get_angle();

int player_set_position(struct point2D position);
int player_set_angle(double angle);
enum items* player_get_items();

int player_move(enum player_moves move);

#endif //G6_PLAYER_H
