// This is a personal academic project. Dear PVS-Studio, please check it.
// PVS-Studio Static Code Analyzer for C, C++ and C#: http://www.viva64.com
#include <graphics.h>
#include <idtLoader.h>
#include <keyboard.h>
#include <lib.h>
#include <moduleLoader.h>
#include <naiveConsole.h>
#include <pipe/pipe.h>
#include <pmm.h>
#include <process.h>
#include <scheduler.h>
#include <semaphore/semaphore.h>
#include <stdint.h>
#include <string.h>
#include <syscall.h>
#include <time.h>

extern uint8_t text;
extern uint8_t rodata;
extern uint8_t data;
extern uint8_t bss;
extern uint8_t endOfKernelBinary;
extern uint8_t endOfKernel;

static const uint64_t PageSize = 0x1000;

static void *const sampleCodeModuleAddress = (void *)0x400000;
static void *const sampleDataModuleAddress = (void *)0x500000;

typedef int (*EntryPoint)();

void clearBSS(void *bssAddress, uint64_t bssSize) {
    memset(bssAddress, 0, bssSize);
}

void *getStackBase() {
    return (void *)((uint64_t)&endOfKernel +
                    PageSize * 8       // The size of the stack itself, 32KiB
                    - sizeof(uint64_t) // Begin at the top of the stack
    );
}

void *initializeKernelBinary() {
    char buffer[10];

    ncPrint("[x64BareBones]");
    ncNewline();

    ncPrint("CPU Vendor:");
    ncPrint(cpuVendor(buffer));
    ncNewline();

    ncPrint("[Loading modules]");
    ncNewline();
    void *moduleAddresses[] = {sampleCodeModuleAddress,
                               sampleDataModuleAddress};

    loadModules(&endOfKernelBinary, moduleAddresses);
    ncPrint("[Done]");
    ncNewline();
    ncNewline();

    ncPrint("[Initializing kernel's binary]");
    ncNewline();

    clearBSS(&bss, &endOfKernel - &bss);

    ncNewline();
    ncPrint("  endOfKernel: 0x");
    ncPrintHex((uint64_t)&endOfKernel);
    ncNewline();
    ncPrint("  text: 0x");
    ncPrintHex((uint64_t)&text);
    ncNewline();
    ncPrint("  rodata: 0x");
    ncPrintHex((uint64_t)&rodata);
    ncNewline();
    ncPrint("  data: 0x");
    ncPrintHex((uint64_t)&data);
    ncNewline();
    ncPrint("  bss: 0x");
    ncPrintHex((uint64_t)&bss);
    ncNewline();

    ncPrint("[Done]");
    ncNewline();
    ncNewline();
    return getStackBase();
}

int main() {

    init_pmm(); // init physical memory manager
    init_scheduler();
    load_idt();
    init_sem_list();
    init_pipes();

    ((EntryPoint)sampleCodeModuleAddress)();

    while (1)
        ;

    return 0;
}
