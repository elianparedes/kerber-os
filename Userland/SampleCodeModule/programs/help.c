#include <kstdio.h>
#include <help.h>

/*
* Prints a list of comands
*/

void help(){
    puts("*************************");
    puts("Kerber-OS version 1.0");
    puts("*************************");
    puts("List of comands: ");
    puts("help: shows available programs");
    puts("inforeg : prints all registers");
    puts("printmem [ptr]: dumps 32 byte long memory beggining at address [ptr]");
    puts("time: displays system day and hour");
    puts("primes: prints prime numbers");
    puts("[p1] | [p2] : splits screen and runs both [p1] and [p2]");
    puts("*************************");
}