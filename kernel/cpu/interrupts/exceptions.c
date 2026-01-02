/*
    exceptions.c

    Implementations of the functions in exceptions.h.
*/

#include "cpu/interrupts/exceptions.h"
#include "klib/logging.h"
#include "klib/bool.h"
#include "klib/asm.h"

void exception_handler(struct CPUState *cpu_state) {
    panic("CPU exception #%d (error code: %d)\r\n"
        "GS: 0x%x, FS: 0x%x, ES: 0x%x, DS: 0x%x\r\n"
        "EDI: 0x%x, ESI: 0x%x, EBP: 0x%x, ESP: 0x%x, EBX: 0x%x, EDX: 0x%x, ECX: 0x%x\r\nEAX: 0x%x\r\n"
        "EIP: 0x%x, CS: 0x%x, EFLAGS: 0x%x, USERESP: 0x%x, SS: 0x%x"
        ,
        cpu_state->int_no, cpu_state->err_code,
    cpu_state->gs, cpu_state->fs, cpu_state->es, cpu_state->ds,
    cpu_state->edi, cpu_state->esi, cpu_state->ebp, cpu_state->esp, cpu_state->ebx, cpu_state->edx, cpu_state->ecx, cpu_state->eax,
    cpu_state->eip, cpu_state->cs, cpu_state->eflags, cpu_state->useresp, cpu_state->ss
    );

    while (true) {
        CLI
        HLT
    }
}