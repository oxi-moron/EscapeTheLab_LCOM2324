#ifndef G6_GRAPHICS_H
#define G6_GRAPHICS_H

#include <lcom/lcf.h>

struct point2D {
    int x; int y;
};

#include "drivers/video.h"
#include "game/map.h"
#include "game/player.h"
#include "graphics_macros.h"
#include <math.h>
#include "images/test_menu.xpm"
#include "images/item.xpm"
#include "images/wall_texture.xpm"

static uint32_t width, height, map_width, map_height;

int (graphics_construct) ();
int (graphics_draw_game) ();
int (graphics_draw_menu) ();

// TODO: declare static
int (graphics_draw_player_camera) ();
int (graphics_draw_map) ();
int (graphics_draw_item_bar) ();
void (load_xpms) ();
int (create_line) (struct point2D* line, double angle);
int (set_background_color) (uint32_t color);
double (get_ray_angle) (int diff);
double (to_radians) (double angle);

#endif //G6_GRAPHICS_H
