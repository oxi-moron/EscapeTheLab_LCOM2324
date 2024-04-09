#ifndef G6_VIDEO_H
#define G6_VIDEO_H

#include <lcom/lcf.h>

int (vg_set_mode) (uint16_t mode);
int (vg_map_vram) (uint16_t mode);

int (vg_draw_pixel) (uint16_t x, uint16_t y, uint32_t color);
int (vg_draw_hline) (uint16_t x, uint16_t y, uint16_t len, uint32_t color);
int (vg_draw_rectangle) (uint16_t x, uint16_t y, uint16_t width, uint16_t height, uint32_t color);

int (vg_set_background_color) (uint32_t color);

int (vg_get_indexed_color) (uint32_t row, uint32_t column, uint32_t first, uint8_t step, uint8_t no_rectangles, uint32_t* color);
int (vg_get_direct_color) (uint32_t row, uint32_t column, uint32_t first, uint8_t step, uint32_t* color);
int (vg_get_rectangle_dimensions) (uint8_t no_rectangles, uint16_t* width, uint16_t* height);

uint32_t R(uint32_t color);
uint32_t G(uint32_t color);
uint32_t B(uint32_t color);

#endif //G6_VIDEO_H
