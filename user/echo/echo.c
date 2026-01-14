#include <cli.h>
#include <stdio.h>

void main() {
    benix_GetCLIArgs();

    if (argc < 2) {
        return;
    }

    for (int i = 1; i < argc; i++) {
        printf("%s ", argv[i]);
    }
    printf("\n");
}