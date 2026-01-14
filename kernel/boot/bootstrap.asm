; bootstrap.asm
;
; This is a small bootstrap that simply calls the kernel and could set up some things before doing that:
; - Canceling interruots
;

[bits 32]

section .text
global _start
extern kmain

_start:
    cli
    mov esp, 0x80000

    call kmain

hang:
    hlt
    jmp hang