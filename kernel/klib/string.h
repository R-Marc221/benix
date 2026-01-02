/*
    string.h

    This header provides functions to work with strings.
*/

#pragma once

#include "klib/types.h"

// Get the length of a null-terminated string
u32 strlen(string s);
// Find the first occurence of a character in a string
string strchr(string s, i32 c);
// Tokenize a string
string strtok(string s, const string delim);