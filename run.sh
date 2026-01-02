#!/bin/bash

set -e

chmod +x build.sh
./build.sh

qemu-system-i386 \
    -drive format=raw,file=images/benix.img,index=0,if=ide \
    -drive format=raw,file=images/floppy2.img,index=1,if=ide \
    -boot c -m 512M