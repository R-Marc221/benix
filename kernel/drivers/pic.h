/*
    pic.h

    Benix PIC driver.
*/

#pragma once

#include "klib/types.h"

// PIC1 => master
// PIC2 => slave

// ports
#define PIC1                    0x20
#define PIC2                    0xa0
#define PIC1_CMD                PIC1
#define PIC1_DATA               (PIC1 + 1)
#define PIC2_CMD                PIC2
#define PIC2_DATA               (PIC2 + 1)

// end of interrupt byte
#define PIC_EOI                 0x20

#define ICW1_ICW4               0x01
#define ICW1_SINGLE             0x02
#define ICW1_INTERVAL4          0x04
#define ICW1_LEVEL              0x08
#define ICW1_INIT               0x10

#define ICW4_8086               0x01
#define ICW4_AUTO               0x02
#define ICW4_BUF_SLAVE          0x08
#define ICW4_BUF_MASTER         0x0c
#define ICW4_SFNM               0x10

#define CASCADE_IRQ             2

struct DriverPIC {
    void (*remap)(i32 offset1, i32 offset2);
    void (*disable)(void);
    void (*send_eoi)(u8 irq);
    void (*set_irq_mask)(u8 irqline);
    void (*clear_irq_mask)(u8 irqline);
};

void init_driver_pic(void);
struct DriverPIC* get_driver_pic(void);