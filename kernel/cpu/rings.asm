[bits 32]

section .text
global enter_usermode

enter_usermode:
    cli

    mov eax, [esp + 4]

    mov ax, 0x23
    mov ds, ax
    mov es, ax
    mov fs, ax
    mov gs, ax

    push 0x23
    push eax
    pushf
    pop eax
    or eax, 0x200
    push eax
    push 0x1b
    push 0x80000
    iret