#include "cpu/interrupts/syscalls.h"
#include "klib/io.h"
#include "consoles/console.h"

/*
    1: print string (printstr)
    2: read character from keyboard (readchar)
    3: print character (printchar)
    4: clear screen (cls)
    5: read file (fread)
    6: list directory (lsdir)
*/
void syscall_handler(struct SyscallRegisters *registers) {
    switch (registers->eax) {
        case 1:
            print((string)registers->ebx);
            break;

        case 2:
            registers->ecx = getchar();
            break;

        case 3:
            putchar(registers->ebx);
            break;

        case 4:
            get_driver_console()->clear();
            break;

        case 5:
            fread((string)registers->ebx, (u8*)registers->ecx, registers->edx);
            break;

        case 6:
            lsdir((string)registers->ebx);
            break;
    }
}