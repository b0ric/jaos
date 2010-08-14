ASM=nasm

all: bootloader

bootloader: bootloader.asm bootloader.ash
	$(ASM) -f bin $< -o $@

.PHONY: clean

clean:
	rm -f bootloader
