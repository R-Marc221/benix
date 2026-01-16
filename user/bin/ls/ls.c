#include <cli.h>
#include <stdio.h>
#include <syscall.h>
#include <string.h>

void main() {
    benix_GetCLIArgs();

    char buffer[224 * 11];

    if (argc < 2) {
        syscall_lsdir("/", buffer);
    } else if (strcmp(argv[1], "-h") == 0 || strcmp(argv[1], "--help") == 0) {
        printf("ls -- displays the name of the files and folders in a directory\n");
        printf("Usage: ls [dir]\n");
        printf("NONE                        : display the content of /\n");
        printf("-h / --help                 : display this message\n");
        printf("[dir]                       : display the content of the directory\n");
        return;
    } else {
        syscall_lsdir(argv[1], buffer);
    }

    printf("%s", buffer);
}