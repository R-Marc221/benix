/*
    gdt.h

    This header provides functions and structures to represent and manage the GDT.
*/

#pragma once

#include "klib/types.h"

#define GDT_ENTRIES                 6

struct __attribute__((packed)) GDTEntry {
    u16 limit_low;
    u16 base_low;
    u8 base_middle;
    u8 access;
    u8 granularity;
    u8 base_high;
};

struct __attribute__((packed)) GDTPointer {
    u16 limit;
    u32 base;
};

struct GDT {
    struct GDTEntry entries[GDT_ENTRIES];
    struct GDTPointer pointer;

    void (*set_entry)(i32 index, u32 base, u32 limit, u8 access, u8 granularity);
};

void init_gdt(void);
void gdt_flush(struct GDTPointer* gdtptr);
struct GDT* get_gdt(void);