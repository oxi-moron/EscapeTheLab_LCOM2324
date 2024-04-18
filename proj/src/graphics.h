#ifndef G6_GRAPHICS_H
#define G6_GRAPHICS_H

#include <lcom/lcf.h>

struct point2D {
    int x; int y;
};

int (draw_current_frame) ();

int (draw_player_camera) ();
int (draw_map) ();

int (create_line) (struct point2D* line, double angle);

#endif //G6_GRAPHICS_H
