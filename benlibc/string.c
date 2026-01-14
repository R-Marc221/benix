#include "string.h"

int strcmp(const char *s1, const char *s2) {
    while ((*s1 != '\0' && *s2 != '\0') && *s1 == *s2) {
        s1++;
        s2++;
    }

    return (*s1 == *s2) ? 0 : (*s1 > *s2) ? 1 : -1;
}

void* memcpy(void* dest, const void* src, int n) {
    unsigned char* pdest = (unsigned char*)dest;
    const unsigned char* psrc = (const unsigned char*)src;

    for (int i = 0; i < n; i++) {
        pdest[i] = psrc[i];
    }

    return dest;
}

void* memset(void* src, int c, int n) {
    unsigned char* p = (unsigned char*)src;

    for (int i = 0; i < n; i++) {
        p[i] = (unsigned char)c;
    }

    return src;
}

void* memmove(void* dest, const void* src, int n) {
    unsigned char* pdest = (unsigned char*)dest;
    const unsigned char* psrc = (const unsigned char*)src;

    if (src > dest) {
        for (int i = 0; i < n; i++) {
            pdest[i] = psrc[i];
        }
    } else if (src < dest) {
        for (int i = n; i > 0; i--) {
            pdest[i-1] = psrc[i-1];
        }
    }

    return dest;
}

int memcmp(const void* s1, const void* s2, int n) {
    const unsigned char* p1 = (const unsigned char*)s1;
    const unsigned char* p2 = (const unsigned char*)s2;

    for (int i = 0; i < n; i++) {
        if (p1[i] != p2[i]) {
            return p1[i] < p2[i] ? -1 : 1;
        }
    }

    return 0;
}

int strlen(const char* s) {
    int i = 0;
    while (s[i] != '\0') {
        i++;
    }
    return i;
}

char* strchr(char* s, int c) {
    while (*s) {
        if (*s == (char)c) {
            return (char*)s;
        }
        s++;
    }

    return NULL;
}

char* strtok(char* s, char* delim) {
    static char* next = NULL;

    if (s != NULL)
        next = s;
    else if (next == NULL)
        return NULL;

    while (*next && strchr(delim, *next))
        next++;

    if (*next == '\0') {
        next = NULL;
        return NULL;
    }

    char* start = next;

    while (*next && !strchr(delim, *next))
        next++;

    if (*next) {
        *next = '\0';
        next++;
    } else {
        next = NULL;
    }

    return start;
}