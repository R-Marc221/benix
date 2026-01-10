# Benix changelog

## Version 0.0.1
- Updated `welcome.c`
- Fixed switch to usermode, so that syscalls do not make #UD happen anymore
- Reorganized syscalls
- `console.h` and `console.c` are now in the `drivers/` directory. `consoles/` has been removed
- The VGA cursor is reset when the screen is cleared using the console driver `clear()` function
- Added basic libc that does not respect any C standard (`benlibc/`)
- Organized makefiles

## Version 0.0.0
- Initial release