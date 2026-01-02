/*
    idt.h

    This header provides functions and structures to represent and manage the IDT.
*/

#pragma once

#include "klib/types.h"

#define IDT_ENTRIES                 256

struct __attribute__((packed)) IDTEntry {
    u16 isr_low;
    u16 kernel_cs;
    u8 reserved;
    u8 attributes;
    u16 isr_high;
};

struct __attribute__((packed)) IDTPointer {
    u16 limit;
    u32 base;
};

struct IDT {
    __attribute__((aligned(0x10))) struct IDTEntry entries[256];
    struct IDTPointer pointer;

    void (*set_entry)(u8 index, voidptr isr, u8 flags);
};

void init_idt(void);
void idt_flush(struct IDTPointer* idtptr);
struct IDT* get_idt(void);