/*
    memory.c

    Implementations of the functions in memory.h.
*/

#include "klib/memory.h"

voidptr memcpy(voidptr dest, const voidptr src, size_t n) {
    u8* pdest = (u8*)dest;
    const u8* psrc = (const u8*)src;

    for (size_t i = 0; i < n; i++) {
        pdest[i] = psrc[i];
    }

    return dest;
}

voidptr memset(voidptr src, i32 c, size_t n) {
    u8* p = (u8*)src;

    for (size_t i = 0; i < n; i++) {
        p[i] = (u8)c;
    }

    return src;
}

voidptr memmove(voidptr dest, const voidptr src, size_t n) {
    u8* pdest = (u8*)dest;
    const u8* psrc = (const u8*)src;

    if (src > dest) {
        for (size_t i = 0; i < n; i++) {
            pdest[i] = psrc[i];
        }
    } else if (src < dest) {
        for (size_t i = n; i > 0; i--) {
            pdest[i-1] = psrc[i-1];
        }
    }

    return dest;
}

i32 memcmp(const voidptr s1, const voidptr s2, size_t n) {
    const u8* p1 = (const u8*)s1;
    const u8* p2 = (const u8*)s2;

    for (size_t i = 0; i < n; i++) {
        if (p1[i] != p2[i]) {
            return p1[i] < p2[i] ? -1 : 1;
        }
    }

    return 0;
}