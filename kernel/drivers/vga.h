/*
    vga.h

    Benix VGA driver.

    Currently only supports VGA text mode.
*/

#pragma once

#include "klib/types.h"

// informations according to VGA specifications
#define STD_VGA_BUFFER                  0xb8000
#define STD_VGA_HEIGHT                  25
#define STD_VGA_WIDTH                   80

#define VGA_CRT_CIR                     0x3d4
#define VGA_CRT_CDR                     0x3d5

#define VGA_CURSOR_START                0x0a
#define VGA_CURSOR_END                  0x0b
#define VGA_CURSOR_HIGH                 0x0e
#define VGA_CURSOR_LOW                  0x0f

// color codes
typedef enum {
    VGA_BLACK                   = 0,
    VGA_BLUE                    = 1,
    VGA_GREEN                   = 2,
    VGA_CYAN                    = 3,
    VGA_RED                     = 4,
    VGA_MAGENTA                 = 5,
    VGA_BROWN                   = 6,
    VGA_LIGHTGRAY               = 7,
    VGA_DARKGRAY                = 8,
    VGA_LIGHTBLUE               = 9,
    VGA_LIGHTGREEN              = 10,
    VGA_LIGHTCYAN               = 11,
    VGA_LIGHTRED                = 12,
    VGA_PINK                    = 13,
    VGA_YELLOW                  = 14,
    VGA_WHITE                   = 15
} vga_color_t;

struct DriverVGA {
    u16 *buffer;
    u32 height;
    u32 width;

    void (*move_cursor)(i32 x, i32 y);
    void (*putchar)(u8 c, vga_color_t fg, vga_color_t bg, u32 x, u32 y);
    void (*clear)(vga_color_t fg, vga_color_t bg);
};

void init_driver_vga(void);
struct DriverVGA* get_driver_vga(void);