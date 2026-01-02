#pragma once

#include "klib/types.h"

#define LOAD_ADDRESS                    0x80000

struct ProgramLoader {
    void (*load)(string path, u32 bytes);
    void (*exec)(void);
};

void init_program_loader(void);
struct ProgramLoader* get_program_loader(void);