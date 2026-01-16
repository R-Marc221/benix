/*
    vga.c

    Implementations of the functions in vga.h.
*/

#include "drivers/vga.h"
#include "klib/asm.h"
#include "klib/types.h"

static struct DriverVGA driver_vga;

static void move_cursor(i32 x, i32 y) {
    u16 offset = y * driver_vga.width + x;

    OUTB(VGA_CRT_CIR, VGA_CURSOR_LOW)
    OUTB(VGA_CRT_CDR, offset & VGA_BYTE_MASK)

    OUTB(VGA_CRT_CIR, VGA_CURSOR_HIGH)
    OUTB(VGA_CRT_CDR, (offset >> VGA_BYTE_SHIFT) & VGA_BYTE_MASK);
}

static void putchar(u8 c, vga_color_t fg, vga_color_t bg, u32 x, u32 y) {
    u16 attr = (bg << VGA_ATTR_BG_SHIFT) | (fg & VGA_ATTR_FG_MASK);
    volatile u16* where;
    where = (volatile u16*)driver_vga.buffer + (y * driver_vga.width + x);
    *where = c | (attr << VGA_BYTE_SHIFT);
}

static void clear(vga_color_t fg, vga_color_t bg) {
    for (u32 y = 0; y < STD_VGA_HEIGHT; y++) {
        for (u32 x = 0; x < STD_VGA_WIDTH; x++) {
            putchar(CHAR_WHITESPACE, fg, bg, x, y);
        }
    }
}

static void scrollup(u32 lines, vga_color_t fg, vga_color_t bg) {
    i32 i;
    i32 last_row = STD_VGA_HEIGHT - 1;

    for (i = 0; i < STD_VGA_SIZE; i++) {
        driver_vga.buffer[i] = driver_vga.buffer[i + (lines * STD_VGA_WIDTH)];
    }

    for (i = last_row * STD_VGA_HEIGHT; i < STD_VGA_SIZE; i++) {
        putchar(CHAR_WHITESPACE, fg, bg, i, last_row);
    }
}

void init_driver_vga(void) {
    driver_vga.buffer = (u16*)STD_VGA_BUFFER;
    driver_vga.height = STD_VGA_HEIGHT;
    driver_vga.width = STD_VGA_WIDTH;

    driver_vga.putchar = putchar;
    driver_vga.move_cursor = move_cursor;
    driver_vga.clear = clear;
    driver_vga.scrollup = scrollup;
}

struct DriverVGA* get_driver_vga(void) {
    return &driver_vga;
}