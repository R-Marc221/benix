/*
    console.c

    Implementations of the functions in console.h.
*/

#include "consoles/console.h"

static struct DriverConsole driver_console;

static void move_cursor(i32 x, i32 y) {
    driver_console.cursor.x = x;
    driver_console.cursor.y = y;
    driver_console.driver_vga->move_cursor(x, y);

    if (0 > x || x > driver_console.width || 0 > y || y > driver_console.height) {
        driver_console.cursor.x = 0;
        driver_console.cursor.y++;
    }
}

static void set_cursor_color(u8 fg, u8 bg) {
    driver_console.cursor.fg = fg;
    driver_console.cursor.bg = bg;
}

static void putchar(u8 c) {
    switch (c) {
        case '\n':
            driver_console.cursor.move(0, driver_console.cursor.y + 1);
            break;

        case '\r':
            driver_console.cursor.move(0, driver_console.cursor.y);
            break;

        case '\b':
            driver_console.cursor.move(driver_console.cursor.x - 1, driver_console.cursor.y);
            break;

        case '\t':
            driver_console.cursor.move(driver_console.cursor.x + 4, driver_console.cursor.y);
            break;

        default:
            driver_console.driver_vga->putchar(c, driver_console.cursor.fg, driver_console.cursor.bg, driver_console.cursor.x, driver_console.cursor.y);
            move_cursor(driver_console.cursor.x + 1, driver_console.cursor.y);
            break;
    }

    if (driver_console.cursor.y >= driver_console.height) {
        driver_console.driver_vga->clear(driver_console.cursor.fg, driver_console.cursor.bg);
        driver_console.cursor.x = 0;
        driver_console.cursor.y = 0;
    }
}

static void print(string s) {
    i32 i = 0;
    while (s[i] != '\0') {
        driver_console.putchar(s[i]);
        i++;
    }
}

static void clear(void) {
    driver_console.driver_vga->clear(driver_console.cursor.fg, driver_console.cursor.bg);
    driver_console.cursor.x = 0;
    driver_console.cursor.y = 0;
}

void init_driver_console(struct DriverConsole *driver_console, struct DriverVGA *driver_vga) {
    driver_console->driver_vga = driver_vga;
    driver_console->cursor = (struct ConsoleCursor){
        .x = 0,
        .y = 0,
        .fg = VGA_WHITE,
        .bg = VGA_BLACK,

        .set_color = set_cursor_color,
        .move = move_cursor
    };
    driver_console->height = driver_vga->height;
    driver_console->width = driver_vga->width;

    driver_console->putchar = putchar;
    driver_console->print = print;
    driver_console->clear = clear;
}

struct DriverConsole* get_driver_console(void) {
    return &driver_console;
}