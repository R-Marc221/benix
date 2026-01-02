#pragma once

#include "klib/types.h"

struct SyscallRegisters {
    u32 gs, fs, es, ds;
    u32 edi, esi, ebp, esp, ebx, edx, ecx, eax;
};

void syscall_handler(struct SyscallRegisters* registers);
void syscall_stub(void);