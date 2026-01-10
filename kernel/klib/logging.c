/*
    logging.c

    Implementations of the functions in logging.h.
*/

#include "klib/logging.h"
#include "klib/io.h"
#include "klib/args.h"
#include "drivers/console.h"

void log(log_level_t level, string s, ...) {
    va_list args;
    va_start(args, s);

    get_driver_console()->cursor.set_color(VGA_DARKGRAY, VGA_BLACK);
    print("[kernel] ");

    switch ((i32)level) {
        case LOG_INFO:
            get_driver_console()->cursor.set_color(VGA_LIGHTGREEN, VGA_BLACK);
            print("info");
            break;

        case LOG_WARN:
            get_driver_console()->cursor.set_color(VGA_YELLOW, VGA_BLACK);
            print("warning");
            break;

        case LOG_ERROR:
            get_driver_console()->cursor.set_color(VGA_LIGHTRED, VGA_BLACK);
            print("error");
            break;

        case LOG_PANIC:
            get_driver_console()->cursor.set_color(VGA_PINK, VGA_BLACK);
            print("panic");
            break;

        case LOG_DEBUG:
            get_driver_console()->cursor.set_color(VGA_LIGHTBLUE, VGA_BLACK);
            print("debug");
            break;

        default:
            break;
    }

    get_driver_console()->cursor.set_color(VGA_LIGHTGRAY, VGA_BLACK);
    print(": ");
    vprintf(s, args);
    print("\n");
    get_driver_console()->cursor.set_color(VGA_WHITE, VGA_BLACK);

    va_end(args);
}

void info(string s, ...) {
    va_list args;
    va_start(args, s);

    get_driver_console()->cursor.set_color(VGA_DARKGRAY, VGA_BLACK);
    print("[kernel] ");
    get_driver_console()->cursor.set_color(VGA_LIGHTGREEN, VGA_BLACK);
    print("info");
    get_driver_console()->cursor.set_color(VGA_LIGHTGRAY, VGA_BLACK);
    print(": ");
    vprintf(s, args);
    print("\n");
    get_driver_console()->cursor.set_color(VGA_WHITE, VGA_BLACK);

    va_end(args);
}

void warning(string s, ...) {
    va_list args;
    va_start(args, s);

    get_driver_console()->cursor.set_color(VGA_DARKGRAY, VGA_BLACK);
    print("[kernel] ");
    get_driver_console()->cursor.set_color(VGA_YELLOW, VGA_BLACK);
    print("warning");
    get_driver_console()->cursor.set_color(VGA_LIGHTGRAY, VGA_BLACK);
    print(": ");
    vprintf(s, args);
    print("\n");
    get_driver_console()->cursor.set_color(VGA_WHITE, VGA_BLACK);

    va_end(args);
}

void error(string s, ...) {
    va_list args;
    va_start(args, s);

    get_driver_console()->cursor.set_color(VGA_DARKGRAY, VGA_BLACK);
    print("[kernel] ");
    get_driver_console()->cursor.set_color(VGA_LIGHTRED, VGA_BLACK);
    print("error");
    get_driver_console()->cursor.set_color(VGA_LIGHTGRAY, VGA_BLACK);
    print(": ");
    vprintf(s, args);
    print("\n");
    get_driver_console()->cursor.set_color(VGA_WHITE, VGA_BLACK);

    va_end(args);
}

void panic(string s, ...) {
    va_list args;
    va_start(args, s);

    get_driver_console()->cursor.set_color(VGA_DARKGRAY, VGA_BLACK);
    print("[kernel] ");
    get_driver_console()->cursor.set_color(VGA_PINK, VGA_BLACK);
    print("panic");
    get_driver_console()->cursor.set_color(VGA_LIGHTGRAY, VGA_BLACK);
    print(": ");
    vprintf(s, args);
    print("\n");
    get_driver_console()->cursor.set_color(VGA_WHITE, VGA_BLACK);

    va_end(args);
}

void debug(string s, ...) {
    va_list args;
    va_start(args, s);

    get_driver_console()->cursor.set_color(VGA_DARKGRAY, VGA_BLACK);
    print("[kernel] ");
    get_driver_console()->cursor.set_color(VGA_LIGHTBLUE, VGA_BLACK);
    print("debug");
    get_driver_console()->cursor.set_color(VGA_LIGHTGRAY, VGA_BLACK);
    print(": ");
    vprintf(s, args);
    print("\n");
    get_driver_console()->cursor.set_color(VGA_WHITE, VGA_BLACK);

    va_end(args);
}