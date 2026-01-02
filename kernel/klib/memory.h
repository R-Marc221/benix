/*
    memory.h

    This header provides functions to manipulate memory easily.
*/

#pragma once

#include "klib/types.h"

// copy a buffer
voidptr memcpy(voidptr dest, const voidptr src, size_t n);
// set a buffer to a given value
voidptr memset(voidptr src, i32 c, size_t n);
// copy a buffer and handle overlaps
voidptr memmove(voidptr dest, const voidptr src, size_t n);
// compare two buffers
i32 memcmp(const voidptr s1, const voidptr s2, size_t n);