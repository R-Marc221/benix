#pragma once

#include "klib/types.h"

struct __attribute__((packed)) TSS {
    u16 previous_task, __previous_task_unused;
    u32 esp0;
    u32 ss0, __ss0_unused;
    u32 esp1;
    u32 ss1, __ss1_unused;
    u32 esp2;
    u32 ss2, __ss2_unused;
    u32 cr3;
    u32 eip, eflags, eax, ecx, edx, ebx, esp, ebp, esi, edi;
    u16 es, __es_unused;
    u16 cs, __cs_unused;
    u16 ss, __ss_unused;
    u16 ds, __ds_unused;
    u16 fs, __fs_unused;
    u16 gs, __gs_unused;
    u16 ldt_selector, __ldt_selector_unused;
    u16 debug_flag, io_map;
};

void init_tss(void);
void tss_flush(u32 ss, u32 esp);
struct TSS* get_tss(void);