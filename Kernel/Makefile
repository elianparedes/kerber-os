include Makefile.inc

KERNEL=kernel.bin
SOURCES=$(shell find . -name "*.c")
SOURCES_ASM=$(wildcard asm/*.asm)
OBJECTS=$(SOURCES:.c=.o)
OBJECTS_ASM=$(SOURCES_ASM:.asm=.o)
LOADERSRC=loader.asm
# Uncomment to use buddy memory allocator
# MEMFLAGS=-D BUDDY
MEMFLAGS=

LOADEROBJECT=$(LOADERSRC:.asm=.o)
STATICLIBS=

all: $(KERNEL)

$(KERNEL): $(LOADEROBJECT) $(OBJECTS) $(STATICLIBS) $(OBJECTS_ASM)
	$(LD) $(LDFLAGS) -T kernel.ld -o $(KERNEL) $(LOADEROBJECT) $(OBJECTS) $(OBJECTS_ASM) $(STATICLIBS)
	$(LD) $(LDFLAGS) -T kernel.ld --oformat=elf64-x86-64 -o kernel.elf $(LOADEROBJECT) $(OBJECTS) $(OBJECTS_ASM) $(STATICLIBS) 

%.o: %.c
	$(GCC) $(GCCFLAGS) $(MEMFLAGS) -I./include -I./include/drivers -I./include/interrupts -I./include/interrupts/exceptions -I./include/interrupts/idt -I./include/interrupts/syscalls -I./include/interrupts/time -I./include/mem -I./include/process -I./include/graphics -I./include/lib -c $< -o $@

%.o : %.asm
	$(ASM) $(ASMFLAGS) $< -o $@

$(LOADEROBJECT):
	$(ASM) $(ASMFLAGS) $(LOADERSRC) -o $(LOADEROBJECT)

clean:
	rm -rf asm/*.o drivers/*.o graphics/*.o process/*.o interrupts/*.o interrupts/*/*.o mem/*.o lib/*.o pipe/*.o semaphore/*.o *.o *.bin *.elf

.PHONY: all clean
