#ifndef G6_GRAPHICS_H
#define G6_GRAPHICS_H

#include <lcom/lcf.h>

struct point2D {
    int x; int y;
};

int (graphics_draw_current_frame) ();

int (draw_player_camera) ();
int (draw_map) ();

int (create_line) (struct point2D* line, double angle);
int (set_background_color) (uint32_t color);

#endif //G6_GRAPHICS_H
