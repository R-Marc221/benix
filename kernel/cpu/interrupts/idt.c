/*
    idt.c

    Implementations of the functions in idt.h (excepted idt_flush()).
*/

#include "cpu/interrupts/idt.h"
#include "cpu/interrupts/syscalls.h"
#include "cpu/interrupts/irqs.h"

extern voidptr isr_stub_table[];
static struct IDT idt;

static void set_entry(u8 index, voidptr isr, u8 flags) {
    struct IDTEntry* entry = &idt.entries[index];

    entry->isr_low = (u32)isr & 0xffff;
    entry->kernel_cs = 0x08;
    entry->attributes = flags;
    entry->isr_high = (u32)isr >> 16;
    entry->reserved = 0;
}

void init_idt(void) {
    idt.set_entry = set_entry;

    idt.pointer.base = (uintptr)&idt.entries[0];
    idt.pointer.limit = (u16)sizeof(struct IDTEntry) * IDT_ENTRIES - 1;

    for (u8 i = 0; i < 32; i++) {
        set_entry(i, isr_stub_table[i], 0x8e);
    }

    set_entry(33, irq1_stub, 0x8e);
    set_entry(0x80, syscall_stub, 0xee);

    idt_flush(&idt.pointer);
}

struct IDT* get_idt(void) {
    return &idt;
}