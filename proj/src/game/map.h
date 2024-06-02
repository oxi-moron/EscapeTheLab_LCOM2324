#ifndef G6_MAP_H
#define G6_MAP_H

#include <lcom/lcf.h>

int (map_get_grid_pos) (uint32_t x, uint32_t y, uint8_t* pos);
int (map_set_grid_pos) (uint32_t x, uint32_t y, uint8_t pos);

int (map_get_grid_dimensions) (uint32_t* width, uint32_t* height);
int (map_load) (uint8_t map_no);

int (map_get_start_pos) (uint32_t* x, uint32_t*y, uint32_t* angle, uint8_t map_no);

#endif //G6_MAP_H
