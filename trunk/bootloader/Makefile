ASM=nasm

all: bootloader

bootloader: bootloader.asm bootloader.h
	$(ASM) -f bin $< -o $@

.PHONY: clean

clean:
	rm -f bootloader
