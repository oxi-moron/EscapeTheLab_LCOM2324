#include "video.h"
#include "VBE.h"

static char *video_mem;

static unsigned h_res;
static unsigned v_res;
static unsigned bits_per_pixel;
static uint8_t red_mask_size, blue_mask_size, green_mask_size;


int (vg_set_mode) (uint16_t mode) {
    reg86_t reg86;
    memset(&reg86, 0, sizeof(reg86));

    reg86.intno = 0x10;
    reg86.ax = 0x4F02;
    reg86.bx = 1 << 14 | mode;

    if (sys_int86(&reg86) != OK) {
        printf("ERROR: %s", __func__);
        return 1;
    }

    return 0;
}

int (vg_map_vram) (uint16_t mode) {

    vbe_mode_info_t vbe_mode_info;

    if (vbe_get_mode_info(mode, &vbe_mode_info) != 0) {
        printf("ERROR: %s", __func__ );
        return 1;
    }

    h_res = vbe_mode_info.XResolution;
    v_res = vbe_mode_info.YResolution;
    bits_per_pixel = vbe_mode_info.BitsPerPixel;

    red_mask_size = vbe_mode_info.RedMaskSize;
    green_mask_size = vbe_mode_info.GreenMaskSize;
    blue_mask_size = vbe_mode_info.BlueMaskSize;

    int r;
    struct minix_mem_range mr;

    unsigned int vram_base = vbe_mode_info.PhysBasePtr;
    unsigned int vram_size = h_res * v_res * bits_per_pixel;

    mr.mr_base = (phys_bytes) vram_base;
    mr.mr_limit = mr.mr_base + vram_size;

    if( OK != (r = sys_privctl(SELF, SYS_PRIV_ADD_MEM, &mr)))
        panic("sys_privctl (ADD_MEM) failed: %d\n", r);

    video_mem = vm_map_phys(SELF, (void *)mr.mr_base, vram_size);

    if(video_mem == MAP_FAILED)
        panic("couldn’t map video memory");

    return 0;
}

int (vg_draw_pixel) (uint16_t x, uint16_t y, uint32_t color) {

    char* pixel = video_mem + (((h_res * y) + x) * (bits_per_pixel / 8));
    if (memcpy(pixel, &color, bits_per_pixel / 8) == NULL) return 1;

    return 0;
}

int (vg_draw_hline) (uint16_t x, uint16_t y, uint16_t len, uint32_t color) {

    for (int i = 0; i < len; i++) {
        vg_draw_pixel(x + i, y, color);
    }

    return 0;
}

int (vg_draw_rectangle) (uint16_t x, uint16_t y, uint16_t width, uint16_t height, uint32_t color) {

    for (int i = 0; i < height; i++) {
        vg_draw_hline(x, y + i, width, color);
    }

    return 0;
}

int (vg_get_indexed_color) (uint32_t row, uint32_t column, uint32_t first, uint8_t step, uint8_t no_rectangles, uint32_t* color) {
    *color = (first + (row * no_rectangles + column) * step) % (1 << bits_per_pixel);

    return 0;
}

int (vg_get_direct_color) (uint32_t row, uint32_t column, uint32_t first, uint8_t step, uint32_t* color) {
    uint32_t red = ((R(first) + column * step) % (1 << red_mask_size)) << (green_mask_size + blue_mask_size);
    uint32_t green = ((G(first) + row * step) % (1 << green_mask_size)) << (blue_mask_size);
    uint32_t blue = (B(first) + (column + row) * step) % (1 << blue_mask_size);
    *color = (red | green | blue);

    return 0;
}

int (vg_get_rectangle_dimensions) (uint8_t no_rectangles, uint16_t* width, uint16_t* height) {
    *width = h_res / no_rectangles;
    *height = v_res / no_rectangles;

    return 0;
}

int (vg_draw_xpm) (uint16_t x, uint16_t y, int16_t width, uint32_t size, uint8_t* pixmap) {
    for (uint32_t i = 0; i < size; i++) {
        vg_draw_pixel(x + i % width, y + i / width, pixmap[i]);
    }

    return 0;
}

uint32_t R(uint32_t color) {
    return (color << (bits_per_pixel - red_mask_size - green_mask_size - blue_mask_size)) >> (bits_per_pixel - red_mask_size);
}

uint32_t G(uint32_t color) {
    return (color << (bits_per_pixel - green_mask_size - blue_mask_size)) >> (bits_per_pixel - green_mask_size);
}

uint32_t B(uint32_t color) {
    return (color << (bits_per_pixel - blue_mask_size)) >> (bits_per_pixel - blue_mask_size);
}

int calculate_next_pos(uint16_t x, uint16_t xf, uint16_t y, uint16_t yf, uint16_t* next_x, uint16_t* next_y, int16_t speed) {
    if (speed < 0) {
        *next_x = x + 1;
        *next_y = y + 1;
    } else {
        *next_x = x + speed;
        *next_y = y + speed;
    }

    if (*next_y > yf) *next_y = yf;
    if (*next_x > xf) *next_x = xf;

    return 0;
}
