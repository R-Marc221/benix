/*
    string.c

    Implementations of the functions in string.h.
*/

#include "klib/string.h"
#include "klib/null.h"

u32 strlen(string s) {
    u32 i = 0;
    while (s[i] != '\0') {
        i++;
    }
    return i;
}

string strchr(string s, i32 c) {
    while (*s) {
        if (*s == (char)c) {
            return (string)s;
        }
        s++;
    }

    return NULL;
}

string strtok(string s, const string delim) {
    static string next = NULL;
    next = s;
    while (*next && strchr(delim, *next)) {
        next++;
    }

    string start = next;
    while (*next && !strchr(delim, *next)) {
        next++;
    }

    if (*next) {
        *next = '\0';
        next++;
    } else {
        next = NULL;
    }

    return start;
}