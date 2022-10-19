#include <semaphore.h>
#include <process.h>
#include <pmm.h>


#define MAX_PROCESS 50

typedef struct sem_t
{
    char * name;
    int value;
    int lock;
    process_t * process_list[MAX_PROCESS]
};

int _xadd(int * var_ptr  , int value);
int _xchg(int * var_ptr  , int value);

int lock = 0;

// SpinLock
static  void acquire(int *lock){
  while(_xchg(lock, 1) != 0);
}

static void release(int *lock){
  _xchg(lock, 0);
}

sem_t * sem_open(const char * name, int value){
    struct sem_t * sem = kmalloc(sizeof(struct sem_t));
    sem->name = name;
    sem->lock = 0;
    return sem;
}

int sem_wait(sem_t * semaphore){
    acquire(&lock);
    if(xadd(semaphore->value, -1) <= 0){
        sleep();
    }
    release(&lock);
}
 
int sem_post(sem_t* semaphore){
    acquire(&lock);
    if (xadd(semaphore->value, 1) > 0){
        wakeup();
    }
    release(&lock);
}

int sem_close(sem_t* semaphore){
    kfree(semaphore);
}
