#include "drivers/keyboard.h"
#include "klib/bool.h"
#include "klib/types.h"
#include "drivers/ps2.h"

static struct DriverKeyboard keyboard;
static const char scancode_set1[136] = {
    0,    27, '1','2','3','4','5','6','7','8','9','0','-','=','\b',
    '\t','q','w','e','r','t','y','u','i','o','p','[',']','\n',
    0, 'a','s','d','f','g','h','j','k','l',';','\'','`', 0, '\\',
    'z','x','c','v','b','n','m',',','.','/', 0, '*', 0, ' ',
    0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
    0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
    0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0
};
static const char scancode_set1_shift[136] = {
    0,    27, '!','@','#','$','%','^','&','*','(',')','_','+','\b',
    '\t','Q','W','E','R','T','Y','U','I','O','P','{','}','\n',
    0,'A','S','D','F','G','H','J','K','L',':','"','~',0,'|',
    'Z','X','C','V','B','N','M','<','>','?',0,'*',0,' ',
    0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
    0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
    0,0,0,0,0,0,0,0,0,0,0,0,0,0
};
static bool shift_pressed = false;

static char scancode_to_char(u8 scancode) {
    if (scancode == SCANCODE_LSHIFT_PRESSED || scancode == SCANCODE_RSHIFT_PRESSED) {
        shift_pressed = true;
        return 0;
    }
    if (scancode == SCANCODE_LSHIFT_RELEASED || scancode == SCANCODE_RSHIFT_RELEASED) {
        shift_pressed = false;
        return 0;
    }

    if (scancode & 0x80) {
        return 0;
    }

    if (shift_pressed) {
        return scancode_set1_shift[scancode];
    } else {
        return scancode_set1[scancode];
    }
}

static char getchar(void) {
    u8 scancode;
    if (!get_driver_ps2()->dequeue(&scancode)) {
        return 0;
    }
    return scancode_to_char(scancode);
}

void init_driver_keyboard(void) {
    keyboard.scancode_to_char = scancode_to_char;
    keyboard.getchar = getchar;
}

struct DriverKeyboard* get_driver_keyboard(void) {
    return &keyboard;
}