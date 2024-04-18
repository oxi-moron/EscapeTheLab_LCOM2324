#include "map.h"

int (create_map) () {
    /*grid = {{1, 1, 1, 1, 1, 1, 1, 1},
            {1, 0, 0, 0, 0, 0, 0, 1},
            {1, 0, 0, 0, 0, 0, 0, 1},
            {1, 0, 0, 0, 0, 0, 0, 1},
            {1, 0, 0, 0, 0, 0, 0, 1},
            {1, 0, 0, 0, 0, 0, 0, 1},
            {1, 0, 0, 0, 0, 0, 0, 1},
            {1, 1, 1, 1, 1, 1, 1, 1}}; */

    map_height = 16;
    map_width = 10;

    return 0;
}

int (get_grid_pos) (uint32_t x, uint32_t y, uint8_t* pos) {
    if (x > map_width - 1 || y > map_height - 1) {
        printf("ERROR: %s, invalid position", __func__);
        return 1;
    }

    *pos = grid[y][x];

    return 0;
}

int (set_grid_pos) (uint32_t x, uint32_t y, uint8_t pos) {
    if (x > map_width - 1 || y > map_height - 1) {
        printf("ERROR: %s, invalid position", __func__);
        return 1;
    }

    grid[y][x] = pos;

    return 0;
}

int (get_grid_dimensions) (uint32_t* width, uint32_t* height) {
    *width = map_width;
    *height = map_height;

    return 0;
}
