#include <cli.h>
#include <stdio.h>
#include <syscall.h>

void main() {
    benix_GetCLIArgs();

    char buffer[224 * 11];
    syscall_lsdir(buffer);

    printf("%s", buffer);
}