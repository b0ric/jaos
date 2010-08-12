
MAKE=make

INCDIR=./include
OBJDIR=./obj
# object directory hierarchy
OBJDIRH=./obj/drivers				\
	./obj/drivers/clock			\
	./obj/drivers/video			\
	./obj/drivers/keyboard

ASM=nasm
ASMFLAGS=-f elf

CXX=gcc
CXXFLAGS=-c -I $(INCDIR)

# ld uses max-page-size option to align sections (just as it
# ought to be for ELF executable), but it also aligns sections
# in the file and it leads to excessive grow of kernel size
# here we avoid it because it's useless for us
LD=ld
LDFLAGS=-s -Ttext=0x1000 -Tdata=0x2000 -z max-page-size=0x100

# sequence is important here!
_OBJS=	entry.o					\
	kmain.o					\
	klib.o					\
	interrupts.o				\
	drivers/clock/timer.o			\
	drivers/video/vgascreen.o		\
	drivers/keyboard/keybd.o

OBJECTS=$(patsubst %,$(OBJDIR)/%,$(_OBJS))

all: kernel

kernel: $(OBJECTS)
	$(LD) $(LDFLAGS) $^ -o $@
	strip -R .comment -R .rel.plt -R .plt -R .got.plt $@

$(OBJDIR)/%.o: %.asm
	$(ASM) $(ASMFLAGS) $^ -o $@

$(OBJDIR)/%.o: %.c
	$(CXX) $(CXXFLAGS) $^ -o $@

$(OBJECTS): | $(OBJDIR)

$(OBJDIR):
	mkdir $(OBJDIR)
	mkdir $(OBJDIRH)

.PHONY: clean

clean:
	rm -rf ./obj 
	rm -f kernel
