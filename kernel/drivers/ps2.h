#pragma once

#include "klib/types.h"

#define PS2_KBD_BUFFER_SIZE             256

#define PS2_PORT_DATA                   0x60
#define PS2_PORT_STATUS                 0x64
#define PS2_PORT_COMMAND                0x64

#define PS2_STATUS_OUTBUF               0x01
#define PS2_STATUS_INBUF                0x02

#define PS2_CMD_READ_CONFIG             0x20
#define PS2_CMD_WRITE_CONFIG            0x60
#define PS2_CMD_SELF_TEST               0xaa
#define PS2_CMD_DISABLE_PORT1           0xad
#define PS2_CMD_ENABLE_PORT1            0xae

#define PS2_SELF_TEST_OK                0x55

#define PS2_KBD_CMD_RESET               0xff
#define PS2_KBD_CMD_SET_SCANCODE        0xf0

#define PS2_KBD_ACK                     0xFa
#define PS2_KBD_SELF_TEST_OK            0xaa

#define PS2_CFG_IRQ1                    0x01
#define PS2_CFG_TRANSLATION             0x40

struct ScancodeBuffer {
    u8 content[PS2_KBD_BUFFER_SIZE];
    u32 head;
    u32 tail;
};

struct DriverPS2 {
    void (*wait_input_buffer_clear)(void);
    void (*wait_output_buffer_full)(void);
    void (*enqueue)(u8 scancode);
    i32 (*dequeue)(u8* scancode);
};

void init_driver_ps2(void);
struct DriverPS2* get_driver_ps2(void);