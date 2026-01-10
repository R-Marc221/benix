#include <syscall.h>

int main() {
    char README_TXT[512];
    char DIR_CONTENT[512];

    syscall_cls();
    syscall_fread("README  TXT", README_TXT, 512);
    syscall_printstr(README_TXT);

    syscall_printstr("\nDirectory content:\n");
    syscall_lsdir(DIR_CONTENT);
    syscall_printstr(DIR_CONTENT);

    syscall_printstr("\nEnter some text: ");
    while (1) {
        int c = syscall_getchar();
        if (c) {
            syscall_printchar(c);
        }
    }

    while (1);
}