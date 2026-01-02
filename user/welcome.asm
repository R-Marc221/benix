[bits 32]

section .text
global _start

_start:
    mov eax, 0x04
    int 0x80

    mov eax, 0x05
    mov ebx, filename
    mov ecx, file_buffer
    mov edx, 512
    int 0x80

    mov eax, 0x01
    mov ebx, file_buffer
    int 0x80

    jmp $

section .data
filename:                   db      "README  TXT"
file_buffer: times 512 db 0