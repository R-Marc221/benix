/*
    console.h

    Kernel VGA text console.
*/

#pragma once

#include "drivers/vga.h"

struct ConsoleCursor {
    u32 x;
    u32 y;
    u8 fg;
    u8 bg;

    void (*move)(i32 x, i32 y);
    void (*set_color)(u8 fg, u8 bg);
};

struct DriverConsole {
    struct DriverVGA* driver_vga;
    struct ConsoleCursor cursor;
    u32 height;
    u32 width;

    void (*putchar)(u8 c);
    void (*print)(string s);
    void (*clear)(void);
};

void init_driver_console(struct DriverConsole* driver_console, struct DriverVGA* driver_vga);
struct DriverConsole* get_driver_console(void);