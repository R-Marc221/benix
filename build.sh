#!/bin/bash

set -e

# make directories
mkdir -p binaries images
mkdir -p binaries/user

# compile kernel
make -C kernel clean && make -C kernel

# compile libc
make -C benlibc clean && make -C benlibc

# compile user programs
make -C user/welcome clean && make -C user/welcome

# assemble bootloader
nasm -f bin -o binaries/bootsect.bin bootloader/bootsect.asm
nasm -f bin -o binaries/stage2.bin bootloader/stage2.asm

# create and format floppy image
dd if=/dev/zero of=images/benix.img bs=512 count=2880 conv=notrunc
dd if=/dev/zero of=images/floppy2.img bs=512 count=2880 conv=notrunc
mkfs.fat -F 12 images/floppy2.img
# copy files on secondary floppy
mcopy -i images/floppy2.img user/README.txt ::
mcopy -i images/floppy2.img binaries/user/welcome.bin ::

# copy bootloader and kernel on floppy image
dd if=binaries/bootsect.bin of=images/benix.img seek=0 conv=notrunc
dd if=binaries/stage2.bin of=images/benix.img seek=1 conv=notrunc
dd if=binaries/kernel.bin of=images/benix.img seek=2 conv=notrunc