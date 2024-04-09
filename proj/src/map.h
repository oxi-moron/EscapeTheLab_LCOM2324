#ifndef G6_MAP_H
#define G6_MAP_H

#include <lcom/lcf.h>

int (create_map) ();

int (get_grid_pos) (uint32_t x, uint32_t y, uint8_t* pos);
int (set_grid_pos) (uint32_t x, uint32_t y, uint8_t pos);

int (get_grid_dimensions) (uint32_t* width, uint32_t* height);

// TODO: delete for better structuring
static uint8_t grid[8][8] = {{1, 1, 1, 1, 1, 1, 1, 1},
                           {1, 0, 0, 0, 0, 0, 0, 1},
                           {1, 0, 0, 0, 0, 0, 0, 1},
                           {1, 0, 0, 0, 0, 0, 0, 1},
                           {1, 0, 0, 0, 0, 0, 0, 1},
                           {1, 0, 0, 0, 0, 0, 0, 1},
                           {1, 0, 0, 0, 0, 0, 0, 1},
                           {1, 1, 1, 1, 1, 1, 1, 1}};
static uint32_t map_width = 8;
static uint32_t map_height = 8;

#endif //G6_MAP_H
