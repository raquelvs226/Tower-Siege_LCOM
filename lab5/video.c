#include <lcom/lcf.h>
#include <stdint.h>
#include <string.h>
#include "video.h"

/* ---------------------------------------------------------------------------
 * Static globals — scoped to this translation unit
 * -------------------------------------------------------------------------*/
static char *video_mem;          /* Virtual address to which VRAM is mapped */
static vbe_mode_info_t vmi;      /* VBE mode info                           */
static unsigned bytes_per_pixel; /* Number of VRAM bytes per pixel          */

/* ---------------------------------------------------------------------------
 * vg_init — map VRAM into process address space
 * -------------------------------------------------------------------------*/
void *(vg_init)(uint16_t mode) {
    /* 1. Get VBE mode info */
    if (vbe_get_mode_info(mode, &vmi) != OK) {
        printf("vbe_get_mode_info() failed\n");
        return NULL;
    }

    unsigned int vram_base  = vmi.PhysBasePtr;
    bytes_per_pixel         = (vmi.BitsPerPixel + 7) / 8;
    unsigned int vram_size  = vmi.XResolution * vmi.YResolution * bytes_per_pixel;

    /* 2. Grant permission to map physical memory */
    struct minix_mem_range mr;
    mr.mr_base  = (phys_bytes) vram_base;
    mr.mr_limit = mr.mr_base + vram_size;

    int r;
    if (OK != (r = sys_privctl(SELF, SYS_PRIV_ADD_MEM, &mr))) {
        panic("sys_privctl (ADD_MEM) failed: %d\n", r);
        return NULL;
    }

    /* 3. Map VRAM into virtual address space */
    video_mem = vm_map_phys(SELF, (void *) mr.mr_base, vram_size);
    if (video_mem == MAP_FAILED) {
        panic("couldn't map video memory");
        return NULL;
    }

    return video_mem;
}

/* ---------------------------------------------------------------------------
 * vg_set_mode — invoke VBE Set Mode (0x4F02)
 * -------------------------------------------------------------------------*/
int vg_set_mode(uint16_t mode) {
    reg86_t reg;
    memset(&reg, 0, sizeof(reg));

    reg.intno = 0x10;
    reg.ah    = 0x4F;
    reg.al    = 0x02;
    reg.bx    = mode | BIT(14); /* linear frame buffer; bit15=0 clears memory */

    if (sys_int86(&reg) != OK) {
        printf("sys_int86() failed\n");
        return 1;
    }
    return 0;
}

/* ---------------------------------------------------------------------------
 * vg_draw_pixel — write color to VRAM at (x, y)
 * -------------------------------------------------------------------------*/
int (vg_draw_pixel)(uint16_t x, uint16_t y, uint32_t color) {
    if (x >= vmi.XResolution || y >= vmi.YResolution)
        return 1;

    unsigned int offset = (y * vmi.XResolution + x) * bytes_per_pixel;
    memcpy(video_mem + offset, &color, bytes_per_pixel);

    return 0;
}

/* ---------------------------------------------------------------------------
 * vg_draw_hline — draw horizontal line from (x,y) of length len
 * -------------------------------------------------------------------------*/
int (vg_draw_hline)(uint16_t x, uint16_t y, uint16_t len, uint32_t color) {
    for (uint16_t i = 0; i < len; i++) {
        if (vg_draw_pixel(x + i, y, color) != 0)
            return 1;
    }
    return 0;
}

/* ---------------------------------------------------------------------------
 * vg_draw_rectangle — filled rectangle with top-left at (x, y)
 * -------------------------------------------------------------------------*/
int (vg_draw_rectangle)(uint16_t x, uint16_t y,
                      uint16_t width, uint16_t height, uint32_t color) {
    for (uint16_t row = 0; row < height; row++) {
        if (vg_draw_hline(x, y + row, width, color) != 0)
            return 1;
    }
    return 0;
}
