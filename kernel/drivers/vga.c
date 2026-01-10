/*
    vga.c

    Implementations of the functions in vga.h.
*/

#include "drivers/vga.h"
#include "klib/asm.h"

static struct DriverVGA driver_vga;

static void move_cursor(i32 x, i32 y) {
    u16 offset = y * driver_vga.width + x;

    OUTB(VGA_CRT_CIR, VGA_CURSOR_LOW)
    OUTB(VGA_CRT_CDR, offset & 0xff)

    OUTB(VGA_CRT_CIR, VGA_CURSOR_HIGH)
    OUTB(VGA_CRT_CDR, (offset >> 8) & 0xff);
}

static void putchar(u8 c, vga_color_t fg, vga_color_t bg, u32 x, u32 y) {
    u16 attr = (bg << 4) | (fg & 0x0f);
    volatile u16* where;
    where = (volatile u16*)driver_vga.buffer + (y * driver_vga.width + x);
    *where = c | (attr << 8);
}

static void clear(vga_color_t fg, vga_color_t bg) {
    for (u32 y = 0; y < STD_VGA_HEIGHT; y++) {
        for (u32 x = 0; x < STD_VGA_WIDTH; x++) {
            putchar(' ', fg, bg, x, y);
        }
    }
}

void init_driver_vga(void) {
    driver_vga.buffer = (u16*)STD_VGA_BUFFER;
    driver_vga.height = STD_VGA_HEIGHT;
    driver_vga.width = STD_VGA_WIDTH;

    driver_vga.putchar = putchar;
    driver_vga.move_cursor = move_cursor;
    driver_vga.clear = clear;
}

struct DriverVGA* get_driver_vga(void) {
    return &driver_vga;
}