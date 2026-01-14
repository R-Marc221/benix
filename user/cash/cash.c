#include <stdbool.h>
#include <stdio.h>
#include <syscall.h>
#include <string.h>
#include <cli.h>

#define NAME                        "CASH"
#define VERSION                     "0.0.0"

#define COMMENT                     '#'
#define CMD_HELP                    "help"
#define CMD_VERSION                 "version"
#define CMD_CLEAR                   "clear"

#define MAX_ARGUMENTS               32

#define MAX_CHARS                   255

#define SHELL_ADDRESS               0x90000
#define PROGRAM_ADDRESS             0x120000

void main() {
    benix_GetCLIArgs();

    char* prompt_text = NAME;
    char buffer[MAX_CHARS];

    char* sh_argv[MAX_ARGUMENTS];
    int sh_argc;

    syscall_cls();

    printf("CASH - Certainly Amazing SHell\n");
    printf("Type `help` for a list of built-in commands.\n\n");

    while (true) {
        printf("%s> ", prompt_text);
        input(buffer);

        if (strcmp(buffer, CMD_HELP) == 0) {
            printf("\nBuilt-in commands\n");
            printf("-----------------------------------------------\n");
            printf("help                    : display this message\n");
            printf("version                 : display shell version\n");
            printf("clear                   : clear the screen\n");
            printf("<filename> [args]       : execute a program\n");
            putchar('\n');
            continue;
        }

        if (strcmp(buffer, CMD_VERSION) == 0) {
            printf(VERSION "\n");
            continue;
        }

        if (strcmp(buffer, CMD_CLEAR) == 0) {
            syscall_cls();
            continue;
        }

        sh_argc = 0;
        char *token = strtok(buffer, " ");

        while (token && sh_argc < MAX_ARGUMENTS) {
            sh_argv[sh_argc++] = token;
            token = strtok(NULL, " ");
        }

        if (sh_argc == 0 || buffer[0] == COMMENT) {
            continue;
        }

        if (syscall_ffind(sh_argv[0])) {
            struct CommandLineArguments* cliargs;
            cliargs->argc = sh_argc;
            cliargs->argv = sh_argv;

            int prog_address;
            if (strcmp(sh_argv[0], "cash") == 0) {
                prog_address = SHELL_ADDRESS;
            } else {
                prog_address = PROGRAM_ADDRESS;
            }

            memmove((void*)CLIARGS_ADDRESS, (void*)cliargs, sizeof(struct CommandLineArguments));
            fread(sh_argv[0], (char*)prog_address, 512 * 10);
            void (*program)(void) = (void (*)())prog_address;
            program();
        } else {
            printf("Invalid command: %s\n", sh_argv[0]);
        }
    }
}