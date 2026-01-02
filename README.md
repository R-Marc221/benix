# The Benix Operating System
Benix is a small 32-bit hobby operating system for x86 I made for fun and learning. It is written in Assembly (Intel syntax) and C. Every part of the OS has been made from scratch, from the bootloader to the user space.

## Features
### Drivers
- PIC
- PS/2 keyboard
- Keyboard
- ATA
- FAT12
- VGA
- Console
### Loadable binary formats
- Flat binary
### Userspace
- Syscalls
- User programs

## Build instructions
First, you need to clone the git repository. If you don't have git, you can install it from your distribution packages.
```sh
git clone https://github.com/wither16x/benix
```
Next, you can either just build it or build and run it in the qemu emulator.
To build it:
```sh
chmod +x build.sh
./build.sh
```
The floppy image should be available in images/benix.img.
To build and run it in qemu:
```sh
chmod +x run.sh
./run.sh
```
**Benix has not been tried on real hardware yet!**

## Documentation
There is no real documentation. Some of the code has comments, but you still need Assembly and C knowledge to understand what it does if it is not clearly explained or not explained at all...

## Contributing
To contribute to Benix:
1. Fork the git repository
2. Apply your changes
3. Test them carefully
4. Describe them in [CHANGELOG.md](CHANGELOG.md)
5. Make a detailed pull request