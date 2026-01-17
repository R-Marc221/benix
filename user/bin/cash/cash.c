#include <stdbool.h>
#include <stdio.h>
#include <syscall.h>
#include <string.h>
#include <cli.h>

#define NAME                        "CASH"
#define VERSION                     "0.0.1"

#define COMMENT                     '#'
#define CMD_HELP                    "help"
#define CMD_VERSION                 "version"
#define CMD_CLEAR                   "clear"

#define MAX_ARGUMENTS               32

#define MAX_CHARS                   1024

#define SHELL_ADDRESS               0x90000
#define PROGRAM_ADDRESS             0x120000

void main() {
    benix_GetCLIArgs();

    char* prompt_text = NAME;
    char buffer[MAX_CHARS];

    char* sh_argv[MAX_ARGUMENTS];
    int sh_argc;

    char prog_path[MAX_CHARS / MAX_ARGUMENTS];
    int prog_address;
    int result;

    syscall_cls();

    printf("CASH - Certainly Amazing SHell\n");
    printf("Type `help` for a list of built-in commands.\n\n");

    while (true) {
        printf("%s> ", prompt_text);
        input(buffer);

        if (buffer[0] == '\0' || buffer[0] == COMMENT) {
            continue;
        }

        sh_argc = 0;
        char* token = strtok(buffer, " ");
        while (token && sh_argc < MAX_ARGUMENTS) {
            sh_argv[sh_argc++] = token;
            token = strtok(NULL, " ");
        }

        if (strcmp(sh_argv[0], CMD_HELP) == 0) {
            printf("\nBuilt-in commands\n");
            printf("-----------------------------------------------\n");
            printf("help                    : display this message\n");
            printf("version                 : display shell version\n");
            printf("clear                   : clear the screen\n");
            printf("<filename> [args]       : execute a program\n");
            putchar('\n');
            continue;
        }

        if (strcmp(sh_argv[0], CMD_VERSION) == 0) {
            printf("%s\n", VERSION);
            continue;
        }

        if (strcmp(sh_argv[0], CMD_CLEAR) == 0) {
            syscall_cls();
            continue;
        }

        snprintf(prog_path, sizeof(prog_path), "%s", sh_argv[0]);
        prog_address = (strcmp(prog_path, "bin/cash") == 0) ? SHELL_ADDRESS : PROGRAM_ADDRESS;

        result = fread(prog_path, (char*)prog_address, 512 * 10);
        if (result < 0) {
            snprintf(prog_path, sizeof(prog_path), "bin/%s", sh_argv[0]);
            prog_address = (strcmp(prog_path, "bin/cash") == 0) ? SHELL_ADDRESS : PROGRAM_ADDRESS;

            result = fread(prog_path, (char*)prog_address, 512 * 10);
            if (result < 0) {
                printf("Invalid command: %s\n", sh_argv[0]);
                continue;
            }
        }

        struct CommandLineArguments cliargs;
        cliargs.argc = sh_argc;
        cliargs.argv = sh_argv;

        memmove((void*)CLIARGS_ADDRESS, &cliargs, sizeof(cliargs));

        void (*program)(void) = (void (*)())prog_address;
        program();
    }
}