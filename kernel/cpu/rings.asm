[bits 32]

section .text
global enter_usermode

enter_usermode:
    cli

    mov ebx, [esp + 4]

    mov ax, 0x23
    mov ds, ax
    mov es, ax
    mov fs, ax
    mov gs, ax

    push 0x23
    push 0x200000
    pushf
    pop eax
    or eax, 0x200
    push eax
    push 0x1b
    push ebx

    iret