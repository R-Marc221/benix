#include <stdio.h>
#include <syscall.h>
#include <cli.h>
#include <string.h>

void main() {
    benix_GetCLIArgs();

    char buffer_name[12];
    char buffer_version[12];
    char buffer_arch[12];
    int name_found, version_found, arch_found;

    printf("\0");   // look at show.c to understand

    if (argc < 2) {
        name_found = fread("sys/name.txt", buffer_name, sizeof(buffer_name));
        version_found = fread("sys/version.txt", buffer_version, sizeof(buffer_version));
        arch_found = fread("sys/arch.txt", buffer_arch, sizeof(buffer_arch));

        if (!name_found || !version_found || !arch_found) {
            printf("Missing information files in sys/\n");
            return;
        }

        printf("Name: %s\nVersion: %s\nArchitecture: %s\n", buffer_name, buffer_version, buffer_arch);
    } else if (strcmp(argv[1], "-h") == 0 || strcmp(argv[1], "--help") == 0) {
        printf("info -- displays informations about the operating system\n");
        printf("Usage: info [option]\n");
        printf("NONE                        : display every available information\n");
        printf("-h / --help                 : display this message\n");
        printf("-n / --name                 : display the system name\n");
        printf("-v / --version              : display the system version\n");
        printf("-a / --architecture         : display the system architecture\n");
        return;
    } else if (strcmp(argv[1], "-n") == 0 || strcmp(argv[1], "--name") == 0) {
        name_found = fread("sys/name.txt", buffer_name, sizeof(buffer_name));
        if (!name_found) {
            printf("Could not find sys/name.txt\n");
            return;
        }

        printf("%s\n", buffer_name);
    } else if (strcmp(argv[1], "-v") == 0 || strcmp(argv[1], "--version") == 0) {
        version_found = fread("sys/version.txt", buffer_version, sizeof(buffer_version));
        if (!version_found) {
            printf("Could not find sys/version.txt\n");
            return;
        }

        printf("%s\n", buffer_version);
    } else if (strcmp(argv[1], "-a") == 0 || strcmp(argv[1], "--architecture") == 0) {
        arch_found = fread("sys/arch.txt", buffer_arch, sizeof(buffer_arch));
        if (!arch_found) {
            printf("Could not find sys/arch.txt\n");
            return;
        }

        printf("%s\n", buffer_arch);
    } else {
        printf("Invalid option: %s\n", argv[1]);
    }
}