/*
    args.h

    This header provides macros to work with variadic parameters.
    (They are just wrappers around built-in macros)
*/

#pragma once

#define va_list                 __builtin_va_list
#define va_start(ap, param)     __builtin_va_start(ap, param)
#define va_end(ap)              __builtin_va_end(ap)
#define va_arg(param, t)        __builtin_va_arg(param, t)