/*
    console.h

    Kernel VGA text console.
*/

#pragma once

#include "drivers/vga.h"

#define CHAR_BACKSPACE                  '\b'
#define CHAR_NEWLINE                    '\n'
#define CHAR_CARRIAGE_RETURN            '\r'
#define CHAR_TAB                        '\t'
#define CHAR_NULL                       '\0'

#define TAB_LENGTH                      4
#define BACKSPACE_LENGTH                1
#define LINES_TO_SCROLL                 1

#define CURSOR_DEFAULT_X                0
#define CURSOR_DEFAULT_Y                0
#define CURSOR_STEP                     1

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
    void (*scrollup)(u32 lines);
};

void init_driver_console(struct DriverVGA* driver_vga);
struct DriverConsole* get_driver_console(void);