#include <string.h>
#include <syscall.h>
#include <cli.h>
#include <stdio.h>

void main() {
    benix_GetCLIArgs();

    char buffer[512 * 10];

    if (argc < 2) {
        printf("Filename required\n");
        return;
    }

    if (strcmp(argv[1], "-h") == 0 || strcmp(argv[1], "--help") == 0) {
        printf("show -- displays the content of a file in ASCII\n");
        printf("Usage: show <filename>\n");
        printf("-h / --help                 : display this message\n");
        return;
    }

    /*
        I don't know why, but when you execute this program for the first time (in your current session),
        the content of the file will not be displayed, because (I don't know why too) the number of bytes
        returned by fread() equals 0. However, every other time, it will work correctly.
        It also works if I call printf() before fread(), so I display a null character. I hope I will figure
        out how to fix this bug... Please tell me if you have any idea!
    */
    printf("\0");
    int result = fread(argv[1], buffer, sizeof(buffer));
    if (result == -1) {
        printf("File not found: %s\n", argv[1]);
    } else if (result == -2) {
        printf("Is a directory: %s\n", argv[1]);
    } else {
        printf("%s", buffer);
    }
}