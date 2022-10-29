/*#include <semaphore/semaphore.h>
#include <fifo_queue.h>
#include <pmm.h>
#include <process.h>

#define MAX_PROCESS 50

typedef struct sem
{
    char * name;
    int value;
    int lock;
    fifo_queue_ptr fifo_queue;
} sem;

static list;

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

static void sleep(sem_ptr sem){
    process_t * current_proc = get_current_process();
    current_proc->status=PAUSED;
    enqueue(sem->fifo_queue, current_proc);
}

static void wakeup(sem_ptr sem){
    process_t * process = (process_t *)dequeue(sem->fifo_queue);
    if (process != NULL){
        process->status= READY;
    }
}

sem_ptr sem_open(const char * name, int value){
    sem_ptr new_sem = kmalloc(sizeof(sem));
    new_sem->name = name;
    new_sem->value=value;
    new_sem->lock = 0;
    new_sem->fifo_queue = new_fifo_queue();
    return new_sem;
}

int sem_wait(sem_ptr sem){
    acquire(&lock);
    if(xadd(sem->value, -1) <= 0){
        sleep(sem);
    }
    release(&lock);
}

int sem_post(sem_ptr sem){
    acquire(&lock);
    if (xadd(sem->value, 1) > 0){
        wakeup(sem);
    }
    release(&lock);
}

int sem_close(sem_ptr sem){
    kfree(sem);
}*/
