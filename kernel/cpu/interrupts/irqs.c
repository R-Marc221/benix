#include "cpu/interrupts/irqs.h"
#include "klib/asm.h"
#include "drivers/ps2.h"
#include "drivers/pic.h"

void irq1_handler(void) {
    u8 scancode = __asm_inb(PS2_PORT_DATA);
    get_driver_ps2()->enqueue(scancode);
    get_driver_pic()->send_eoi(1);
}