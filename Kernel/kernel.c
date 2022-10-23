// This is a personal academic project. Dear PVS-Studio, please check it.
// PVS-Studio Static Code Analyzer for C, C++ and C#: http://www.viva64.com
#include <idtLoader.h>
#include <keyboard.h>
#include <graphics.h>
#include <lib.h>
#include <moduleLoader.h>
#include <naiveConsole.h>
#include <pmm.h>
#include <process.h>
#include <scheduler.h>
#include <stdint.h>
#include <string.h>
#include <time.h>
#include <syscall.h>
#include <semaphore/semaphore.h>

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

int my_yield(){
    return 0;
}

void slowInc(int64_t *p, int64_t inc){
  uint64_t aux = *p;
  my_yield(); //This makes the race condition highly probable
  aux += inc;
  *p = aux;
}


int64_t counter = 0;
sem_ptr sem;

void process_a(){

    for(int i = 0 ; i < 1000000 ; i++){
        sem_wait(sem);
        counter++;
        sem_post(sem);
    }
    exit_process();
    
}

void process_b(){

    for(int i = 0 ; i < 1000000 ; i++){
        sem_wait(sem);
        counter--;
        sem_post(sem);
    }
    exit_process();
}

void kernel_shell(){

    sem = sem_open("my_sem",1);

    add_process(process_a,NULL);
    add_process(process_b,NULL);

    int prev = ticks_elapsed();
    int new = prev;
        while(new - prev < 200){
            new = ticks_elapsed();
        }

    printf("El valor del counter es: %d",counter);

    while(1){

    }
}

int main() {
    init_pmm(); // init physical memory manager
    load_idt();

    full_screen_distribution();
    add_process(kernel_shell,NULL);

    while (1)
        ;

    return 0;
}
