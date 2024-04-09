#include "graphics.h"
#include "video.h"
#include "map.h"
#include <math.h>

int (draw_current_frame) () {

    if (draw_player_camera() != 0) {
        printf("ERROR: %s", __func__ );
        return 1;
    }

    return 0;
}

int (draw_player_camera) () {

    int width = 800;
    //int height = 600;

    for (int i = 0; i < width; i++) {
        // double ray_angle = 0;
        struct point2D* line = (struct point2D*)malloc(sizeof(struct point2D) * 256);
        int line_size = create_line(line);
        if (line_size < 0) {
            printf("ERROR: %s", __func__ );
            free(line);
            return 1;
        }

        for (int i = 0; i < line_size; i++) {
            vg_draw_pixel(line[i].x, line[i].y, 0xFF0000);
        }
        free(line);
    }

    return 0;
}

// TODO: index is sus
int (create_line) (struct point2D* line) {
    int index = 0;
    int x1 = 400;
    int y1 = 300;
    int distance = 1000;
    int x2 = (int) (x1 + distance * cos(0));
    int y2 = (int) (y1 + distance * sin(0));
    int dx = abs(x2 - x1);
    int dy = abs(y2 - y1);
    int sx = (x1 < x2) ? 1 : -1;
    int sy = (y1 < y2) ? -1 : 1;

    int err = dx - dy;

    while ((x1 != x2 || y1 != y2) && index < 256) {
        uint8_t pos;
        if (get_grid_pos(x1 / 100, y1 / 75, &pos) != 0) {
            printf("ERROR: %s", __func__ );
            return -1;
        }
        if (pos == 1) {
            return index;
        }

        line[index].x = x1; line[index].y = y1;
        index++;

        int e2 = 2 * err;
        if (e2 > -dy) {
            err -= dy;
            x1 += sx;

        }
        if (e2 < dx) {
            err += dx;
            y1 += sy;
        }
    }
    return index;
}
