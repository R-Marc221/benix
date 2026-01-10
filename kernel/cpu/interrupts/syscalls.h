#pragma once

#include "klib/types.h"

struct SyscallRegisters {
    u32 edi, esi, ebp, esp;
    u32 ebx, edx, ecx, eax;
};

void syscall_handler(struct SyscallRegisters* r);
void syscall_stub(void);