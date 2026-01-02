/*
    types.h

    This header provides typedefs. We don't use stdint.h because the kernel compiles with the
    -nostdinc flag.
*/

#pragma once

// signed types
typedef signed char                 i8;
typedef signed short                i16;
typedef signed int                  i32;
typedef signed long long            i64;

// unsigned types
typedef unsigned char               u8;
typedef unsigned short              u16;
typedef unsigned int                u32;
typedef unsigned long long          u64;

// boolean type
typedef _Bool                       bool;

// pointer types
typedef void*                       voidptr;
typedef char*                       string;
typedef unsigned char*              ustring;

// special types
typedef i32                         size_t;
typedef u32                         usize_t;
typedef i32                         inptr;
typedef u32                         uintptr;