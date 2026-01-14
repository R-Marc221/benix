#pragma once

#define NULL                    0

int strcmp(const char* s1, const char* s2);
void* memcpy(void* dest, const void* src, int n);
void* memset(void* src, int c, int n);
void* memmove(void* dest, const void* src, int n);
int memcmp(const void* s1, const void* s2, int n);
int strlen(const char* s);
char* strchr(char* s, int c);
char* strtok(char* s, char* delim);