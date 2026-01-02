NASM        := nasm
CC			:= gcc
LD          := ld

ASMFLAGS    := -f elf32
CFLAGS		:= -m32 -ffreestanding -nostdlib -nostdinc -fno-pic -fno-pie -g -c -Ikernel
LDFLAGS     := -m elf_i386 -T linker.ld

SRC_DIR     := kernel
OBJ_DIR     := obj
TARGET      := binaries/kernel.bin

ASM_FILES   := $(shell find $(SRC_DIR) -name '*.asm')
SD_FILES	:= $(shell find $(SRC_DIR) -name '*.c')

ASM_OBJ := $(patsubst $(SRC_DIR)/%.asm,$(OBJ_DIR)/%.o,$(ASM_FILES))
SD_OBJ := $(patsubst $(SRC_DIR)/%.c,$(OBJ_DIR)/%.o,$(SD_FILES))

OBJ_FILES := $(ASM_OBJ) $(SD_OBJ)

.PHONY: all clean

all: $(TARGET)

$(TARGET): $(OBJ_FILES)
	$(LD) $(LDFLAGS) -o $@ $^

$(OBJ_DIR)/%.o: $(SRC_DIR)/%.asm
	@mkdir -p $(dir $@)
	$(NASM) $(ASMFLAGS) $< -o $@

$(OBJ_DIR)/%.o: $(SRC_DIR)/%.c
	@mkdir -p $(dir $@)
	$(CC) $(CFLAGS) $< -o $@

clean:
	rm -rf $(OBJ_DIR) $(TARGET)