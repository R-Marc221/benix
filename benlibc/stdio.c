#include "stdio.h"
#include "syscall.h"
#include "stdlib.h"

void putchar(int c) {
    if (c) {
        syscall_printchar(c);
    }
}

void puts(const char* s) {
    syscall_printstr((char*)s);
    syscall_printchar('\n');
}

void vprintf(const char* fmt, va_list args) {
    char buffer[32];

    while (*fmt) {
        if (*fmt == '%') {
            fmt++;

            switch (*fmt) {
                case 'c': {
                    char ch = (char)va_arg(args, int);
                    putchar(ch);
                    break;
                }

                case 's': {
                    const char* str = va_arg(args, const char*);
                    syscall_printstr((char*)str);
                    break;
                }

                case 'b': {
                    int i = va_arg(args, int);
                    itoa(i, buffer, 2);
                    syscall_printstr(buffer);
                    break;
                }

                case 'o': {
                    int i = va_arg(args, int);
                    itoa(i, buffer, 8);
                    syscall_printstr(buffer);
                    break;
                }

                case 'd': {
                    int i = va_arg(args, int);
                    itoa(i, buffer, 10);
                    syscall_printstr(buffer);
                    break;
                }

                case 'x': {
                    int i = va_arg(args, int);
                    itoa(i, buffer, 16);
                    syscall_printstr(buffer);
                    break;
                }
                
                case '%':
                    putchar('%');
                    break;

                default:
                    break;
            }
        } else {
            putchar(*fmt);
        }

        fmt++;
    }
}

void printf(const char* fmt, ...) {
    va_list args;
    va_start(args, fmt);
    vprintf(fmt, args);
    va_end(args);
}

int getchar() {
    return syscall_getchar();
}

void input(char *buf) {
    int i = 0;
    char c = 0;
    while (c != '\n') {
        c = getchar();
        if (c) {
            if (c == '\b') {
                if (i == 0) {
                    continue;
                }
                i--;
                putchar(c);
                putchar(' ');
                putchar('\b');
                continue;
            }
            putchar(c);
            buf[i++] = c;
        }
    }
    buf[i - 1] = '\0';
}

int fread(const char* path, char *buf, int size) {
    return syscall_fread((char*)path, buf, size);
}