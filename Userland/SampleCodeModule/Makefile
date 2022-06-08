include ../Makefile.inc

MODULE=0000-sampleCodeModule.bin
SOURCES=$(wildcard [^_]*.c)
SOURCES_ASM=$(wildcard asm/*.asm)
PROGRAMS_SOURCE=$(shell find ./programs/ -name "*.c")
PROGRAMS_OBJ=$(PROGRAMS_SOURCE:.c=.o)
OBJECTS=$(SOURCES:.c=.o)
OBJECTS_ASM=$(SOURCES_ASM:.asm=.o)

all: $(MODULE)

$(MODULE): $(OBJECTS) $(OBJECTS_ASM) $(PROGRAMS_OBJ) _loader.o
	$(LD) $(LDFLAGS) -T sampleCodeModule.ld _loader.o $(OBJECTS) $(OBJECTS_ASM) $(PROGRAMS_OBJ) -o ../$(MODULE)
	$(LD) $(LDFLAGS) -T sampleCodeModule.ld --oformat=elf64-x86-64 -o _loader.o $(OBJECTS) $(OBJECTS_ASM) $(PROGRAMS_OBJ) -o ../0000-sampleCodeModule.elf

_loader.o: _loader.c
	$(GCC) $(GCCFLAGS) -I./include -c $< -o $@

%.o: %.c
	$(GCC) $(GCCFLAGS) -I./include -c $< -o $@

%.o : %.asm
	$(ASM) $(ASMFLAGS) $< -o $@

clean:
	rm -rf asm/*.o *.o *.bin

.PHONY: all clean print

