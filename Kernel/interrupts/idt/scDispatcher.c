#include <interrupts/scDispatcher.h>
#include <interrupts/syscalls.h>
#include <naiveConsole.h>

uint8_t syscall_dispatcher(uint64_t arg0, uint64_t arg1, uint64_t arg2, uint64_t arg3, uint64_t arg4, uint64_t arg5, uint64_t id){
    int syscall_id= (int)id;
    if (syscall_id == SYSCALL_READ){
        return read((int)arg0, (char*)arg1, arg2);
    }
    else if (syscall_id == SYSCALL_WRITE){
        return write((int)arg0, (char*)arg1, arg2);
    }
    return 0;
}
