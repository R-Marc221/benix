; idt_flush.asm
;
; Implementation of idt_flush() from idt.h.
;

[bits 32]

section .text
global idt_flush

idt_flush:
    mov eax, [esp + 4]
    lidt [eax]

    ret