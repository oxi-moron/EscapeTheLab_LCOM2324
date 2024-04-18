#include "graphics.h"
#include "video.h"
#include "map.h"
#include <math.h>

// TODO: this is global for now
int player_x = 330, player_y = 300;
double player_angle = 60;
uint32_t wall_colors[16] = {0xF7CAC9, 0xA2D5F2, 0xFFF79A, 0xD5C7BC, 0xC2B280, 0x98FF98,
                            0xFFB6C1, 0xFFD700, 0x87CEEB,0xFFA07A, 0x77DD77, 0xF0E68C,
                            0xE6E6FA, 0xFFE4B5, 0xEEE8AA, 0xD8BFD8};


int (draw_current_frame) () {

    if (vg_set_background_color(0x000000) != 0) {
        printf("ERROR: %s", __func__ );
        return 1;
    }

    if (draw_map() != 0) {
        printf("ERROR: %s", __func__ );
        return 1;
    }

    if (draw_player_camera() != 0) {
        printf("ERROR: %s", __func__ );
        return 1;
    }
    return 0;
}

double to_radians(double angle) {
    return 2 * M_PI * angle * 1.0 / 360;
}

uint32_t get_color (double distance_to_wall) {
    uint8_t brightness = (int) ceil(-0.255 * distance_to_wall + 255);
    return (brightness) | (brightness << 8) | (brightness << 16);
}

double get_ray_angle(int diff) {
    return (player_angle - (double) 70 / 2 + ((double) (70 * diff) / 800));
}

int (draw_player_camera) () {

    uint32_t width, height, map_width, map_height;
    if (vg_get_resolution(&width, &height) != 0) {
        printf("ERROR: %s", __func__ );
        return 1;
    }
    if (get_grid_dimensions(&map_width, &map_height) != 0) {
        printf("ERROR: %s", __func__ );
        return 1;
    }

    int cellsize = height / map_height;

    for (uint32_t i = 0; i < width; i++) {
        double ray_angle = get_ray_angle(i);
        struct point2D* line = (struct point2D*)malloc(sizeof(struct point2D) * 256);
        int line_size = create_line(line, ray_angle);
        if (line_size <= 0) {
            printf("ERROR: %s", __func__ );
            free(line);
            return 1;
        }

        for (int i = 0; i < line_size; i++) {
            vg_draw_pixel(line[i].x / 5, (4 * height / 5) + (line[i].y / 5), 0xFF0000);
        }

        struct point2D collision_point = line[line_size - 1];
        double distance_to_wall = sqrt(pow(collision_point.x - player_x, 2) + pow(collision_point.y - player_y, 2));
        distance_to_wall *= fabs(cos(to_radians(ray_angle - player_angle)));
        //uint32_t color = get_color(distance_to_wall);

        int wall_height = (int) ((height * cellsize) / distance_to_wall);
        int draw_start = -wall_height / 2 + height / 2;
        if (draw_start < 0) draw_start = 0;
        int16_t draw_end = wall_height / 2 + height / 2;
        if (draw_end >= (int)height) draw_end = height - 1;

        vg_draw_vline_colormap(width - i, draw_start, draw_end - draw_start, wall_colors);
        free(line);
    }

    return 0;
}

// TODO: index is sus, some values still hardcoded
int (create_line) (struct point2D* line, double angle) {
    int index = 0;
    int x1 = player_x;
    int y1 = player_y;
    int distance = 1000;
    int x2 = (int) (x1 + distance * cos(to_radians(angle)));
    int y2 = (int) (y1 + distance * sin(to_radians(angle)));
    int dx = abs(x2 - x1);
    int dy = abs(y2 - y1);
    int sx = (x1 < x2) ? 1 : -1;
    int sy = (y1 < y2) ? -1 : 1;

    int err = dx - dy;

    while ((x1 != x2 || y1 != y2) && index < 256) {
        uint8_t pos;
        if (get_grid_pos(x1 / 50, y1 / 60, &pos) != 0) {
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

int (draw_map) () {
    uint32_t width, height, map_width, map_height;
    if (vg_get_resolution(&width, &height) != 0) {
        printf("ERROR: %s", __func__ );
        return 1;
    }
    if (get_grid_dimensions(&map_width, &map_height) != 0) {
        printf("ERROR: %s", __func__ );
        return 1;
    }

    for (uint32_t y = 0; y < map_height; y++) {
        for (uint32_t x = 0; x < map_width; x++) {
            uint8_t pos;
            if (get_grid_pos(x, y, &pos) != 0) {
                printf("ERROR: %s", __func__ );
                return 1;
            }
            uint32_t color = pos == 1 ? 0x00FF00 : 0xFFFFFF;
            if (vg_draw_rectangle(x * (width / 5 / map_width), (4 * height / 5) + (y * (height / 5 / map_height))
                              , (width / 5 / map_width), (height / 5 / map_height), color) != 0) {
                printf("ERROR: %s", __func__ );
                return 1;
            }
        }
    }
    return 0;
}
