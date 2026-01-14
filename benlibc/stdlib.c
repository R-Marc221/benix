#include "stdlib.h"
#include "stdbool.h"

void itoa(int i, char* buffer, int base) {
    char* p = buffer;
    bool negative = false;

    if (i == 0) {
        *p++ = '0';
        *p = '\0';
        return;
    }

    if (i < 0 && base == 10) {
        negative = true;
        i = -i;
    }

    while (i) {
        int digit = i % base;
        *p++ = (digit < 10) ? (digit + '0') : (digit - 10 + 'a');
        i /= base;
    }

    if (negative) {
        *p++ = '-';
    }

    *p = '\0';

    char* start = buffer;
    char* end = p - 1;
    while (start < end) {
        char tmp = *start;
        *start++ = *end;
        *end-- = tmp;
    }
}