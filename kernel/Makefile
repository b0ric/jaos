
MAKE=make

INCDIR=./include
# object directory hierarchy
VPATH=	./drivers				\
	./drivers/clock				\
	./drivers/tty

ASM=nasm
ASMFLAGS=-f elf -g

CXX=gcc
CXXFLAGS=-c -I $(INCDIR) -g

# ld uses max-page-size option to align sections (just as it
# ought to be for ELF executable), but it also aligns sections
# in the file and it leads to excessive grow of kernel size
# here we avoid it because it's useless for us
LD=ld
#LDFLAGS=-s -Ttext=0x1000 -Tdata=0x3000 -z max-page-size=0x100
LDFLAGS=-Ttext=0x1000 -Tdata=0x3000 -z max-page-size=0x100

# sequence is important here!
OBJECTS=entry.o					\
	kmain.o					\
	klib.o					\
	interrupts.o				\
	drivers/clock/timer.o			\
	drivers/tty/console.o			\
	drivers/tty/keybd.o			\
	drivers/tty/term.o			\
	drivers/tty/tty.o			\
	drivers/tty/vga.o			\
	drivers/tty/layout/us-std.o


all: kernel

kernel: $(OBJECTS)
	$(LD) $(LDFLAGS) $^ -o $@
#	strip -R .comment -R .rel.plt -R .plt -R .got.plt $@

# NOTE that NASM search for included files in the current directory
# but for make utility it is always the same - dir where Makefile is placed
# to solve this problem here we do a little trick with 'make' function $(dir )
%.o: %.asm
	$(ASM) $(ASMFLAGS) $^ -o $@ -I$(dir $^)

%.o: %.c
	$(CXX) $(CXXFLAGS) $^ -o $@

.PHONY: clean

clean:
	rm -f $(OBJECTS)
	rm -f kernel
