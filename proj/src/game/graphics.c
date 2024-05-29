#include "graphics.h"
#include "cursor.h"

static xpm_image_t items[INVENTORY_SIZE];
static xpm_image_t menu_xpm, wall_texture_xpm, cursor_xpm, pause_menu_xpm, door_texture_xpm, defeat_xpm;

int (graphics_construct) () {
    if (vg_get_resolution(&width, &height) != 0) {
        printf("ERROR: %s\n", __func__ );
        return 1;
    }
    if (map_get_grid_dimensions(&map_width, &map_height) != 0) {
        printf("ERROR: %s\n", __func__ );
        return 1;
    }
    load_xpms();
    return 0;
}

int (graphics_draw_game) () {
    if (set_background_color(BLACK) != 0) {
        printf("ERROR: %s\n", __func__ );
        return 1;
    }
    if (graphics_draw_player_camera() != 0) {
        printf("ERROR: %s\n", __func__ );
        return 1;
    }
    if (graphics_draw_map() != 0) {
        printf("ERROR: %s\n", __func__ );
        return 1;
    }
    if (graphics_draw_item_bar() != 0) {
        printf("ERROR: %s\n", __func__ );
        return 1;
    }

    swap_buffer();
    return 0;
}

int (graphics_draw_menu) () {
    if (vg_draw_xpm(0, 0, menu_xpm.width, menu_xpm.size, menu_xpm.bytes) != 0) {
        printf("ERROR: %s\n", __func__ );
        return 1;
    }

    if (graphics_draw_cursor() != 0) {
        printf("ERROR: %s\n", __func__ );
        return 1;
    }

    swap_buffer();
    return 0;
}

int (graphics_draw_pause_menu) () {
    if (vg_draw_xpm(0, 0, pause_menu_xpm.width, pause_menu_xpm.size, pause_menu_xpm.bytes) != 0) {
        printf("ERROR: %s\n", __func__ );
        return 1;
    }

    if (graphics_draw_cursor() != 0) {
        printf("ERROR: %s\n", __func__ );
        return 1;
    }

    swap_buffer();
    return 0;
}

int (graphics_draw_defeat_screen) () {
    if (vg_draw_xpm(0, 0, defeat_xpm.width, defeat_xpm.size, defeat_xpm.bytes) != 0) {
        printf("ERROR: %s\n", __func__ );
        return 1;
    }

    if (graphics_draw_cursor() != 0) {
        printf("ERROR: %s\n", __func__ );
        return 1;
    }

    swap_buffer();
    return 0;
}


int (graphics_draw_player_camera) () {
    int cellsize = height / map_height;

    for (uint32_t i = 0; i < width; i++) {
        uint8_t delimiter;
        double ray_angle = get_ray_angle(i);
        struct point2D* line = (struct point2D*)malloc(sizeof(struct point2D) * LINE_SIZE);
        int line_size = create_line(line, ray_angle, &delimiter);
        if (line_size <= 0) {
            printf("ERROR: %s\n", __func__ );
            free(line);
            return 1;
        }

        struct point2D collision_point = line[line_size - 1];
        double distance_to_wall = sqrt(pow(collision_point.x - player_get_position().x, 2) + pow(collision_point.y - player_get_position().y, 2));
        distance_to_wall *= fabs(cos(to_radians(ray_angle - player_get_angle())));

        int wall_height = (int) ((height * cellsize) / distance_to_wall);
        int draw_start = -wall_height / 2 + height / 2;
        if (draw_start < 0) draw_start = 0;
        int16_t draw_end = wall_height / 2 + height / 2;
        if (draw_end >= (int)height) draw_end = height - 1;

        if (line_size == LINE_SIZE - 1) {
            vg_draw_vline(width - i, draw_start, draw_end - draw_start, 0x000000);
        } else {
            uint32_t colors[16];
            if (delimiter == 1) {
                for (int j = 0; j < 16; j++) {
                    uint32_t color = (wall_texture_xpm.bytes[(i % 16 + 16 * j) * 3 + 2] << 16)
                                     | (wall_texture_xpm.bytes[(i % 16 + 16 * j) * 3 + 1] << 8)
                                     | wall_texture_xpm.bytes[(i % 16 + 16 * j) * 3];
                    colors[j] = color;
                }
            } else if (delimiter == 2) {
                for (int j = 0; j < 16; j++) {
                    uint32_t color = (door_texture_xpm.bytes[(i % 16 + 16 * j) * 3 + 2] << 16)
                                     | (door_texture_xpm.bytes[(i % 16 + 16 * j) * 3 + 1] << 8)
                                     | door_texture_xpm.bytes[(i % 16 + 16 * j) * 3];
                    colors[j] = color;
                }
            }
            vg_draw_vline_colormap(width - i, draw_start, draw_end - draw_start, colors);
        }
        free(line);
    }

    return 0;
}

int (graphics_draw_item_bar) () {
    enum items* player_items = player_get_items();
    for (int i = 0; i < INVENTORY_SIZE; i++) {
        if (vg_draw_xpm(MINIMAP_WIDTH + (((width - MINIMAP_WIDTH) / INVENTORY_SIZE) * i), height - MINIMAP_HEIGHT / 2,
                        items[player_items[i]].width, items[player_items[i]].size, items[player_items[i]].bytes) != 0) {
            printf("ERROR: %s\n", __func__ );
            return 1;
        }
    }
    return 0;
}

int (graphics_draw_map) () {
    for (uint32_t y = 0; y < map_height; y++) {
        for (uint32_t x = 0; x < map_width; x++) {
            uint8_t pos;
            if (map_get_grid_pos(x, y, &pos) != 0) {
                printf("ERROR: %s\n", __func__ );
                return 1;
            }
            uint32_t color = pos == 1 ? GREEN : WHITE;
            if (vg_draw_rectangle(x * (width / 5 / map_width), (4 * height / 5) + (y * (height / 5 / map_height))
                    , (width / 5 / map_width), (height / 5 / map_height), color) != 0) {
                printf("ERROR: %s\n", __func__ );
                return 1;
            }
        }
    }

    struct point2D player = player_get_position();
    if (vg_draw_rectangle(player.x * MINIMAP_WIDTH / width, (height - MINIMAP_HEIGHT) + player.y * MINIMAP_HEIGHT / height,
                          5, 5, 0xFF0000) != 0) {
        printf("ERROR: %s\n", __func__ );
        return 1;
    }
    return 0;
}

int (graphics_draw_cursor) () {
    uint16_t x, y;
    cursor_get_position(&x, &y);

    if (vg_draw_xpm(x, y, cursor_xpm.width, cursor_xpm.size, cursor_xpm.bytes) != 0) {
        printf("ERROR: %s\n", __func__ );
        return -1;
    }

    return 0;
}

int (create_line) (struct point2D* line, double angle, uint8_t* delimiter) {
    int index = 0;
    int x1 = player_get_position().x;
    int y1 = player_get_position().y;
    int x2 = (int) (x1 + LINE_DISTANCE * cos(to_radians(angle)));
    int y2 = (int) (y1 + LINE_DISTANCE * sin(to_radians(angle)));
    int dx = abs(x2 - x1);
    int dy = abs(y2 - y1);
    int sx = (x1 < x2) ? 1 : -1;
    int sy = (y1 < y2) ? -1 : 1;

    int err = dx - dy;

    while ((x1 != x2 || y1 != y2) && index < LINE_SIZE - 1) {
        uint8_t pos;
        if (map_get_grid_pos(x1 * map_width / width, y1 * map_height / height, &pos) != 0) {
            printf("ERROR: %s\n", __func__ );
            return -1;
        }

        if (pos == 1 || pos == 2) {
            line[index].x = x1; line[index].y = y1;
            index++;
            *delimiter = pos;
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

int (set_background_color) (uint32_t color) {
    if (vg_draw_rectangle(0, 0, width, height, color) != 0) {
        printf("ERROR: %s\n", __func__ );
        return 1;
    }
    return 0;
}

void (load_xpms) () {
    xpm_load(item, XPM_8_8_8, &items[ITEM1]);
    xpm_load(test_menu, XPM_8_8_8, &menu_xpm);
    xpm_load(wall_texture, XPM_8_8_8, &wall_texture_xpm);
    xpm_load(cursor, XPM_8_8_8, &cursor_xpm);
    xpm_load(pause_menu, XPM_8_8_8, &pause_menu_xpm);
    xpm_load(brick_wall, XPM_8_8_8, &door_texture_xpm);
    xpm_load(pause_menu, XPM_8_8_8, &defeat_xpm);
}

double (to_radians) (double angle) {
    return 2 * M_PI * angle * 1.0 / 360;
}

double (get_ray_angle) (int diff) {
    return (player_get_angle() - (double) PLAYER_FOV / 2 + ((double) (PLAYER_FOV * diff) / width));
}
