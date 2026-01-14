#pragma once

#include "stdargs.h"

void putchar(int c);
void puts(const char* s);
void vprintf(const char* fmt, va_list args);
void printf(const char* fmt, ...);
int getchar();
void input(char* buf);
int fread(const char* path, char* buf, int size);