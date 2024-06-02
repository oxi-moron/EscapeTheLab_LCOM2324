#ifndef G6_PLAYER_H
#define G6_PLAYER_H

#include <lcom/lcf.h>
#include <math.h>

#include "game_macros.h"
#include "map.h"

struct point2D {
    int x; int y;
};

struct Player {
    struct point2D position;
    double angle;
};

enum player_moves {UP, DOWN, LEFT, RIGHT, ROTATE_LEFT, ROTATE_RIGHT};

static struct Player player;

void player_construct();

struct point2D player_get_position();
double player_get_angle();

int player_set_position(struct point2D position);
int player_set_angle(double angle);
void player_reset_position();
bool player_crossed_door();

int player_move(enum player_moves move);

/**
 * @brief Converts an angle in degrees to radians.
 * @param angle Angle to be converted.
 * @return Corresponding radians value.
 */
double (to_radians) (double angle);

#endif //G6_PLAYER_H
