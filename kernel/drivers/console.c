/*
    console.c

    Implementations of the functions in console.h.
*/

#include "drivers/console.h"

static struct DriverConsole driver_console;

static void move_cursor(i32 x, i32 y) {
    driver_console.cursor.x = x;
    driver_console.cursor.y = y;
    driver_console.driver_vga->move_cursor(x, y);

    if (0 > x || x > driver_console.width || 0 > y || y > driver_console.height) {
        driver_console.cursor.x = CURSOR_DEFAULT_X;
        driver_console.cursor.y++;
    }
}

static void set_cursor_color(u8 fg, u8 bg) {
    driver_console.cursor.fg = fg;
    driver_console.cursor.bg = bg;
}

static void putchar(u8 c) {
    switch (c) {
        case CHAR_NEWLINE:
            driver_console.cursor.move(CURSOR_DEFAULT_X, driver_console.cursor.y + 1);
            break;

        case CHAR_CARRIAGE_RETURN:
            driver_console.cursor.move(CURSOR_DEFAULT_X, driver_console.cursor.y);
            break;

        case CHAR_BACKSPACE:
            if (driver_console.cursor.x > 0)
                driver_console.cursor.move(driver_console.cursor.x - BACKSPACE_LENGTH, driver_console.cursor.y);
            break;

        case CHAR_TAB:
            driver_console.cursor.move(driver_console.cursor.x + TAB_LENGTH, driver_console.cursor.y);
            break;

        default:
            driver_console.driver_vga->putchar(c, driver_console.cursor.fg, driver_console.cursor.bg, driver_console.cursor.x, driver_console.cursor.y);
            move_cursor(driver_console.cursor.x + CURSOR_STEP, driver_console.cursor.y);
            break;
    }

    if (driver_console.cursor.y >= driver_console.height) {
        driver_console.scrollup(LINES_TO_SCROLL);
        move_cursor(CURSOR_DEFAULT_X, driver_console.height - 1);
    }
}

static void print(string s) {
    i32 i = 0;
    while (s[i] != CHAR_NULL) {
        driver_console.putchar(s[i]);
        i++;
    }
}

static void clear(void) {
    driver_console.driver_vga->clear(driver_console.cursor.fg, driver_console.cursor.bg);
    driver_console.cursor.move(CURSOR_DEFAULT_X, CURSOR_DEFAULT_Y);
}

static void scrollup(u32 lines) {
    driver_console.driver_vga->scrollup(lines, driver_console.cursor.fg, driver_console.cursor.bg);
}

void init_driver_console(struct DriverVGA *driver_vga) {
    driver_console.driver_vga = driver_vga;
    driver_console.cursor = (struct ConsoleCursor){
        .x = 0,
        .y = 0,
        .fg = VGA_WHITE,
        .bg = VGA_BLACK,

        .set_color = set_cursor_color,
        .move = move_cursor
    };
    driver_console.height = driver_vga->height;
    driver_console.width = driver_vga->width;

    driver_console.putchar = putchar;
    driver_console.print = print;
    driver_console.clear = clear;
    driver_console.scrollup = scrollup;
}

struct DriverConsole* get_driver_console(void) {
    return &driver_console;
}