#include "drivers/ps2.h"
#include "klib/asm.h"

static struct DriverPS2 ps2;
static struct ScancodeBuffer scancode_buffer = {
    .head = 0,
    .tail = 0
};

static void wait_input_buffer_clear(void) {
    while (__asm_inb(PS2_PORT_STATUS) & PS2_STATUS_INBUF);
}

static void wait_output_buffer_full(void) {
    while (!(__asm_inb(PS2_PORT_STATUS) & PS2_STATUS_OUTBUF));
}

static void enqueue(u8 scancode) {
    u32 next = (scancode_buffer.head + 1) % PS2_KBD_BUFFER_SIZE;
    if (next == scancode_buffer.tail) {
        return;
    }
    scancode_buffer.content[scancode_buffer.head] = scancode;
    scancode_buffer.head = next;
}

static i32 dequeue(u8* scancode) {
    if (scancode_buffer.head == scancode_buffer.tail) {
        return 0;
    }
    *scancode = scancode_buffer.content[scancode_buffer.tail];
    scancode_buffer.tail = (scancode_buffer.tail + 1) % PS2_KBD_BUFFER_SIZE;
    return 1;
}

void init_driver_ps2(void) {
    ps2.wait_input_buffer_clear  = wait_input_buffer_clear;
    ps2.wait_output_buffer_full = wait_output_buffer_full;
    ps2.enqueue = enqueue;
    ps2.dequeue = dequeue;

    // Disable first PS/2 port
    wait_input_buffer_clear();
    OUTB(PS2_PORT_COMMAND, PS2_CMD_DISABLE_PORT1);

    // Flush output buffer
    while (__asm_inb(PS2_PORT_STATUS) & PS2_STATUS_OUTBUF) {
        (void)__asm_inb(PS2_PORT_DATA);
    }

    // Get controller config
    wait_input_buffer_clear();
    OUTB(PS2_PORT_COMMAND, PS2_CMD_READ_CONFIG);
    wait_output_buffer_full();
    u8 config = __asm_inb(PS2_PORT_DATA);

    // Enable IRQ1, disable translation
    config |= PS2_CFG_IRQ1;
    config &= ~PS2_CFG_TRANSLATION;

    // Update controller config
    wait_input_buffer_clear();
    OUTB(PS2_PORT_COMMAND, PS2_CMD_WRITE_CONFIG);
    wait_input_buffer_clear();
    OUTB(PS2_PORT_DATA, config);

    // Controller self-test
    wait_input_buffer_clear();
    OUTB(PS2_PORT_COMMAND, PS2_CMD_SELF_TEST);
    wait_output_buffer_full();
    if (__asm_inb(PS2_PORT_DATA) != PS2_SELF_TEST_OK) {
        return;
    }

    // Reset keyboard
    wait_input_buffer_clear();
    OUTB(PS2_PORT_DATA, PS2_KBD_CMD_RESET);
    wait_output_buffer_full();
    if (__asm_inb(PS2_PORT_DATA) != PS2_KBD_ACK) {
        return;
    }

    // Check keyboard self-test result
    wait_output_buffer_full();
    if (__asm_inb(PS2_PORT_DATA) != PS2_KBD_SELF_TEST_OK) {
        return;
    }

    // Set scancode set
    wait_input_buffer_clear();
    OUTB(PS2_PORT_DATA, PS2_KBD_CMD_SET_SCANCODE);
    wait_output_buffer_full();
    if (__asm_inb(PS2_PORT_DATA) != PS2_KBD_ACK) {
        return;
    }

    // Try scancode sets 1, 2 and 3
    // Give up of none of them is accepted
    for (u8 set = 1; set <= 3; ++set) {
        wait_input_buffer_clear();
        OUTB(PS2_PORT_DATA, set);
        wait_output_buffer_full();
        if (__asm_inb(PS2_PORT_DATA) == PS2_KBD_ACK) {
            break;
        }
        if (set == 3) {
            return;
        }
    }

    // Re-enable first PS/2 port
    wait_input_buffer_clear();
    OUTB(PS2_PORT_COMMAND, PS2_CMD_ENABLE_PORT1);
}

struct DriverPS2* get_driver_ps2(void) {
    return &ps2;
}