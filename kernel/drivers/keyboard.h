#pragma once

#include "klib/types.h"

#define SCANCODE_LSHIFT_PRESSED                 0x2a
#define SCANCODE_RSHIFT_PRESSED                 0x36
#define SCANCODE_LSHIFT_RELEASED                0xaa
#define SCANCODE_RSHIFT_RELEASED                0xb6

struct DriverKeyboard {
    char (*scancode_to_char)(u8 scancode);
    char (*getchar)(void);
};

void init_driver_keyboard(void);
struct DriverKeyboard* get_driver_keyboard(void);