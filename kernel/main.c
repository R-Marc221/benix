/*
    main.c

    This program contains the kernel entry point (kmain). It initializes all the drivers, does some tests
    and enables interrupts. It ends by an infinite loop on the HLT instruction to save power.
*/

#include "consoles/console.h"
#include "cpu/gdt.h"
#include "cpu/interrupts/idt.h"
#include "cpu/rings.h"
#include "cpu/tss.h"
#include "drivers/keyboard.h"
#include "drivers/pic.h"
#include "drivers/ps2.h"
#include "klib/bool.h"
#include "klib/asm.h"
#include "drivers/vga.h"
#include "drivers/ata.h"
#include "drivers/fs/fat12.h"
#include "klib/logging.h"
#include "proc/loader.h"

void kmain(void) {
    // initialize VGA driver and VGA console
    struct DriverVGA* kdriver_vga = get_driver_vga();
    init_driver_vga(kdriver_vga);

    struct DriverConsole *kdriver_console = get_driver_console();
    init_driver_console(kdriver_console, kdriver_vga);

    info("initialized VGA driver");
    info("initialized VGA console");

    // initialize the GDT and the IDT
    init_gdt();
    info("initialized and loaded new GDT");

    init_idt();
    info("initialized and loaded IDT");

    // initialize the PIC driver, remap the PIC and make sure to mask all the IRQs
    init_driver_pic();
    info("initialized PIC driver");
    get_driver_pic()->remap(0x20, 0x28);
    info("remapped PIC at 0x20, 0x28");
    get_driver_pic()->disable();
    info("disabled all IRQs");

    // initialize the ATA driver, clear the ATA IRQ (14) mask and reset the ATA controller
    init_driver_ata();
    info("initialized ATA driver");
    get_driver_pic()->clear_irq_mask(14);
    info("cleared ATA IRQ (14)");
    get_driver_ata()->reset();
    info("reset disk");
    // debug
    u16 ata_buffer[SECTOR_SIZE / 2];
    get_driver_ata()->identify(ata_buffer);
    debug("ATA device type: 0x%x", ata_buffer[0]);
    u16 bootsector[SECTOR_SIZE / 2];
    get_driver_ata()->read(0, bootsector);
    debug("first word on disk: 0x%x", bootsector[0]);

    // initialize the PS/2 driver and clear the PS/2 IRQ (1) mask
    init_driver_ps2();
    info("initialized PS/2 driver");
    get_driver_pic()->clear_irq_mask(1);
    info("cleared PS/2 keyboard IRQ (1)");

    // initialize the keyboard driver
    init_driver_keyboard();
    info("initialized keyboard driver");

    // initialize the FAT12 driver
    init_fsdriver_fat12();
    info("initialized FAT12 filesystem driver");

    init_program_loader();
    get_program_loader()->load("WELCOME BIN", 512*50);
    info("loaded user program");

    init_tss();
    info("initialized TSS");

    // enable interrupts
    STI

    // switch to usermode
    enter_usermode(get_tss()->esp0);

    // loop
    while (true) {
        HLT
    }
}