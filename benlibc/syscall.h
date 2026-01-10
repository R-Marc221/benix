#pragma once

void syscall_printchar(int c);
void syscall_printstr(char* s);
void syscall_cls(void);
void syscall_fread(char* path, char* buf, int size);
void syscall_lsdir(char* buf);
int syscall_getchar(void);