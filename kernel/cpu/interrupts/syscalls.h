#pragma once

#include "klib/types.h"

struct SyscallRegisters {
    u32 edi;
    u32 esi;
    u32 ebp;
    u32 esp_kernel;
    u32 ebx;
    u32 edx;
    u32 ecx;
    u32 eax;

    u32 gs;
    u32 fs;
    u32 es;
    u32 ds;

    u32 eip;
    u32 cs;
    u32 eflags;
    u32 useresp;
    u32 ss;
};

void syscall_handler(struct SyscallRegisters* r);
void syscall_stub(void);