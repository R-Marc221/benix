#include <stdio.h>
#include <syscall.h>
#include <cli.h>

void main() {
    benix_GetCLIArgs();

    char buffer[512];
    printf("\0");   // look at show.c to understand
    fread("osinfo.txt", buffer, sizeof(buffer));

    if (argc < 2) {
        printf("%s\n", buffer);
    }

    if (!syscall_ffind("osinfo.txt")) {
        printf("osinfo.txt not found\n");
        return;
    }
}