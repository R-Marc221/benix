/*
    pic.c

    Implementations of the functions in pic.h.
*/

#include "drivers/pic.h"
#include "klib/asm.h"

static struct DriverPIC pic;

static void remap(i32 offset1, i32 offset2) {
    OUTB(PIC1_CMD, ICW1_INIT | ICW1_ICW4)
    iowait();
    OUTB(PIC2_CMD, ICW1_INIT | ICW1_ICW4)
    iowait();
    OUTB(PIC1_DATA, offset1)
    iowait();
    OUTB(PIC2_DATA, offset2)
    iowait();
    OUTB(PIC1_DATA, 1 << CASCADE_IRQ)
    iowait();
    OUTB(PIC2_DATA, 2)
    iowait();
    OUTB(PIC1_DATA, ICW4_8086)
    iowait();
    OUTB(PIC2_DATA, ICW4_8086)
    iowait();
    OUTB(PIC1_DATA, 0)
    OUTB(PIC2_DATA, 0)
}

static void disable(void) {
    OUTB(PIC1_DATA, 0xff)
    OUTB(PIC2_DATA, 0xff)
}

static void send_eoi(u8 irq) {
    if (irq >= 8) {
        OUTB(PIC2_CMD, PIC_EOI)
    }
    
    OUTB(PIC1_CMD, PIC_EOI)
}

static void set_irq_mask(u8 irqline) {
    u16 port;
    u8 value;

    if (irqline < 8) {
        port = PIC1_DATA;
    } else {
        port = PIC2_DATA;
        irqline -= 8;
    }

    value = __asm_inb(port) | (1 << irqline);
    OUTB(port, value)
}

static void clear_irq_mask(u8 irqline) {
    u16 port;
    u8 value;

    if (irqline < 8) {
        port = PIC1_DATA;
    } else {
        port = PIC2_DATA;
        irqline -= 8;
    }

    value = __asm_inb(port) & ~(1 << irqline);
    OUTB(port, value)
}

void init_driver_pic(void) {
    pic.remap = remap;
    pic.disable = disable;
    pic.send_eoi = send_eoi;
    pic.set_irq_mask = set_irq_mask;
    pic.clear_irq_mask = clear_irq_mask;
}

struct DriverPIC* get_driver_pic(void) {
    return &pic;
}