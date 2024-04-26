#ifndef G6_VIDEO_H
#define G6_VIDEO_H

#include <lcom/lcf.h>

int (vg_start) (uint16_t mode);
int (vg_set_mode) (uint16_t mode);
int (vg_map_vram) (uint16_t mode);

int (vg_get_resolution) (uint32_t* hres, uint32_t* vres);

int (vg_draw_pixel) (uint16_t x, uint16_t y, uint32_t color);
int (vg_draw_hline) (uint16_t x, uint16_t y, uint16_t len, uint32_t color);
int (vg_draw_vline) (uint16_t x, uint16_t y, uint16_t len, uint32_t color);
int (vg_draw_rectangle) (uint16_t x, uint16_t y, uint16_t width, uint16_t height, uint32_t color);

int (vg_draw_vline_colormap) (uint16_t x, uint16_t y, uint16_t len, uint32_t* color_map);

int (vg_draw_xpm) (uint16_t x, uint16_t y, int16_t width, uint32_t size, uint8_t* pixmap);
void (swap_buffer) ();
void (vg_free_buffer) ();

#endif //G6_VIDEO_H
