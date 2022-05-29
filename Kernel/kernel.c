#include <stdint.h>
#include <string.h>
#include <lib.h>
#include <moduleLoader.h>
#include <naiveConsole.h>
#include <interrupts/time.h>
#include <interrupts/idtLoader.h>
#include <process/process.h>
#include <process/scheduler.h>
#include <drivers/keyboard.h>
#include <mem/pmm.h>

extern uint8_t text;
extern uint8_t rodata;
extern uint8_t data;
extern uint8_t bss;
extern uint8_t endOfKernelBinary;
extern uint8_t endOfKernel;

static const uint64_t PageSize = 0x1000;

static void * const sampleCodeModuleAddress = (void*)0x400000;
static void * const sampleDataModuleAddress = (void*)0x500000;

typedef int (*EntryPoint)();


void clearBSS(void * bssAddress, uint64_t bssSize)
{
	memset(bssAddress, 0, bssSize);
}

void sync_tickprint(char p_ltr){
	int prev = ticks_elapsed();
	while (1){
		int new = ticks_elapsed();
		if (prev != new){
			ncPrintChar(p_ltr);
			ncPrintChar(' ');
			prev = new;
		}
	}
}

void processAFunction(){
	sync_tickprint('A');
}

void processBFunction(){
	sync_tickprint('B');
}

void * getStackBase()
{
	return (void*)(
		(uint64_t)&endOfKernel
		+ PageSize * 8				//The size of the stack itself, 32KiB
		- sizeof(uint64_t)			//Begin at the top of the stack
	);
}

void * initializeKernelBinary()
{
	char buffer[10];

	ncPrint("[x64BareBones]");
	ncNewline();

	ncPrint("CPU Vendor:");
	ncPrint(cpuVendor(buffer));
	ncNewline();

	ncPrint("[Loading modules]");
	ncNewline();
	void * moduleAddresses[] = {
		sampleCodeModuleAddress,
		sampleDataModuleAddress
	};

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

int main()
{	
	ncPrint("[Kernel Main]");
	ncNewline();
	ncPrint("  Sample code module at 0x");
	ncPrintHex((uint64_t)sampleCodeModuleAddress);
	ncNewline();
	ncPrint("  Calling the sample code module returned: ");
	ncPrintHex(((EntryPoint)sampleCodeModuleAddress)());
	ncNewline();

	ncPrint("  Sample data module at 0x");
	ncPrintHex((uint64_t)sampleDataModuleAddress);
	ncNewline();
	ncPrint("  Sample data module contents: ");
	ncPrint((char*)sampleDataModuleAddress);
	ncNewline();

	init_pmm(); // init physical memory manager

	/**
	 * Process scheduling test
	 */
	
    uint64_t processA_base = kmalloc(sizeof(process_t));
	process_t * processA = new_process(processA_base, &processAFunction);

	uint64_t processB_base = kmalloc(sizeof(process_t));
	process_t * processB = new_process(processB_base, &processBFunction);

	add_process(processA);
	add_process(processB);

	load_idt();
	kbd_install();

	while (1){
		/** ... */
	}
	
	ncPrint("[Finished]");
	return 0;
}
