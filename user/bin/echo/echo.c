#include <cli.h>
#include <stdio.h>
#include <string.h>

void main() {
    benix_GetCLIArgs();

    if (argc < 2) {
        return;
    }

    if (strcmp(argv[1], "-h") == 0 || strcmp(argv[1], "--help") == 0) {
        printf("echo -- displays a string followed by a newline\n");
        printf("Usage: echo [text]\n");
        printf("NONE                        : display a newline\n");
        printf("-h / --help                 : display this message\n");
        printf("[text]                      : display the given text\n");
        return;
    }

    for (int i = 1; i < argc; i++) {
        printf("%s ", argv[i]);
    }
    printf("\n");
}