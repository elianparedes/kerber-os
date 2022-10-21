/*#include <semaphore/semaphore.h>
#include <process.h>
#include <pmm.h>

#define MAX_PROCESS 50

typedef struct sem
{
    char * name;
    int value;
    int lock;
    process_t * process_list[MAX_PROCESS]
} sem;

int _xadd(int * var_ptr  , int value);
int _xchg(int * var_ptr  , int value);

int lock = 0;

// spinlock 
static void acquire(int *lock){
  while(_xchg(lock, 1) != 0);
}

static void release(int *lock){
  _xchg(lock, 0);
}

static void sleep(){
    process_t * current_proc = get_current_process();
    current_proc->status=PAUSED;
}

static void wakeup(){
    // wakeup from process_list
}

sem_ptr sem_open(const char * name, int value){
    sem_ptr new_sem = kmalloc(sizeof(sem));
    new_sem->name = name;
    new_sem->value=value;
    new_sem->lock = 0;
    return new_sem;
}

int sem_wait(sem_ptr sem){
    acquire(&lock);
    if(xadd(sem->value, -1) <= 0){
        sleep();
    }
    release(&lock);
}
 
int sem_post(sem_ptr sem){
    acquire(&lock);
    if (xadd(sem->value, 1) > 0){
        wakeup();
    }
    release(&lock);
}

int sem_close(sem_ptr sem){
    kfree(sem);
}*/
