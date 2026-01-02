; gdt.asm
;
; This is a GDT to switch to protected mode.
;

[bits 16]

gdt:
    db 0, 0, 0, 0, 0, 0, 0, 0
gdt_cs:
    db 0xff, 0xff, 0x0, 0x0, 0x0, 10011011b, 11011111b, 0
gdt_ds:
    db 0xff, 0xff, 0x0, 0x0, 0x0, 10010011b, 11011111b, 0
gdtend:

gdtptr:
    dw gdtend - gdt - 1
    dd gdt