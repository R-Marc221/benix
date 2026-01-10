/*
    gdt.c

    Implementations of the functions in gdt.h (excepted gdt_flush()).
*/

#include "cpu/gdt.h"

static struct GDT gdt;

static void set_entry(i32 index, u32 base, u32 limit, u8 access, u8 granularity) {
    gdt.entries[index].base_low = (base & 0xffff);
    gdt.entries[index].base_middle = (base >> 16) & 0xff;
    gdt.entries[index].base_high = (base >> 24) & 0xff;
    gdt.entries[index].limit_low = (limit & 0xffff);
    gdt.entries[index].granularity = ((limit >> 16) & 0x0f) | (granularity & 0xf0);
    gdt.entries[index].access = access;
}

void init_gdt(void) {
    gdt.set_entry = set_entry;

    gdt.pointer = (struct GDTPointer){
        .limit = sizeof(gdt.entries) - 1,
        .base = (u32)&gdt.entries
    };

    set_entry(0, 0, 0, 0, 0);                   // null
    set_entry(1, 0, 0xfffff, 0x9a, 0xc0);       // kernel cs
    set_entry(2, 0, 0xfffff, 0x92, 0xc0);       // kernel ds
    set_entry(3, 0, 0xfffff, 0xfa, 0xcf);       // user cs
    set_entry(4, 0, 0xfffff, 0xf2, 0xcf);       // user ds

    gdt_flush(&gdt.pointer);
}

struct GDT* get_gdt(void) {
    return &gdt;
}