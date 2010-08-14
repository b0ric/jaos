# create bootable floppy image to boot OS
MAKE=make

# filenames for the kernel and bootloader
KERNEL=./kernel/kernel
BOOTLOADER=./bootloader/bootloader

# name of image and it's parameters
IMAGE=boot.img
SECTORS=2880

all: $(IMAGE)

# Put this string to make fs on the floppy image
# but make sure that it's supported by the bootloader (for now it doesn't)
#mke2fs -F -b 1024 $(IMG_NAME)
$(IMAGE): subdirs
	dd if=/dev/zero of=$(IMAGE) bs=512 count=$(SECTORS)
	dd if=$(BOOTLOADER) of=$(IMAGE) bs=512 seek=0 count=1 conv=notrunc
	dd if=$(KERNEL) of=$(IMAGE) bs=512 seek=1 conv=notrunc

subdirs:
	$(MAKE) -C ./bootloader/
	$(MAKE) -C ./kernel/

.PHONY: clean

clean:
	$(MAKE) clean -C  ./bootloader/
	$(MAKE) clean -C ./kernel/
	rm -f $(KERNEL)
	rm -f $(BOOTLOADER)
	rm -f $(IMAGE)
