#include "video.h"

static char *video_mem;

static unsigned h_res;
static unsigned v_res;
static unsigned bits_per_pixel;

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

    char* pixel = video_mem + (((h_res * x) + y) * (bits_per_pixel / 8));
    *pixel = color;

    return 0;
}

int (vg_draw_hline) (uint16_t x, uint16_t y, uint16_t len, uint32_t color) {

    for (int i = 0; i < len; i++) {
        vg_draw_pixel(x, y + i, color);
    }

    return 0;
}

int (vg_draw_rectangle) (uint16_t x, uint16_t y, uint16_t width, uint16_t height, uint32_t color) {

    for (int i = 0; i < height; i++) {
        vg_draw_hline(x + i, y, width, color);
    }

    return 0;
}
