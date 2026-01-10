#include "cpu/interrupts/syscalls.h"
#include "klib/io.h"
#include "drivers/console.h"

/*
    1: print character (printchar)
    2: print string (printstr)
    3: clear screen (cls)
    4: read file (fread)
    5: read directory content (lsdir)
    6: read character from keyboard (readchar)

    Bad syscall: EAX = -1
*/
void syscall_handler(struct SyscallRegisters* r) {
    switch (r->eax) {

        case 1:
            putchar((char)r->ebx);
            break;

        case 2:
            print((string)r->ebx);
            break;

        case 3:
            get_driver_console()->clear();
            break;

        case 4:
            fread((string)r->ebx, (u8*)r->ecx, r->edx);
            break;

        case 5:
            lsdir((string)r->ebx);
            break;

        case 6:
            r->eax = getchar();
            break;

        default:
            r->eax = (u32)-1;
            break;
    }
}