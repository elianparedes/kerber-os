#include <semaphore/semaphore.h>
#include <scheduler.h>
#include <pmm.h>
#include <fifo_queue.h>

#define MAX_PROCESS 50

typedef struct sem
{
    char * name;
    int value;
    int lock;
    fifo_queue_ptr fifo_queue;
} sem;

extern int _xadd(int * var_ptr  , int value);
extern int _xchg(int * var_ptr  , int value);

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

sem_ptr sem_open(char * name, int value){
    sem_ptr new_sem = kmalloc(sizeof(sem));
    new_sem->name = name;
    new_sem->value=value;
    new_sem->lock = 0;
    new_sem->fifo_queue = new_fifo_queue();
    return new_sem;
}

int sem_wait(sem_ptr sem){
    acquire(&sem->lock);
    if(_xadd(&(sem->value), -1) <= 0){
        sleep(sem);
    }
    release(&sem->lock);

    /**
     * TODO: 
     * @return 0 on success
     * -1 on error
     *  EINVAL sem is not a valid semaphore. (Unix sets errno)
    */
    return 0;
}
 
int sem_post(sem_ptr sem){
    acquire(&sem->lock);
    if (_xadd(&(sem->value), 1) > 0){
        wakeup(sem);
    }
    release(&sem->lock);

    /**
     * TODO: 
     * @return 0 on success
     * -1 on error
     *  EINVAL sem is not a valid semaphore. (Unix sets errno)
     * EOVERFLOW
        The maximum allowable value for a semaphore would be exceeded.
    */
    return 0;
}

int sem_close(sem_ptr sem){
    kfree(sem);

    /**
     * TODO: 
     * @return 0 on success
     * -1 on error
     *  EINVAL sem is not a valid semaphore. (Unix sets errno)
     * EOVERFLOW
        The maximum allowable value for a semaphore would be exceeded.
    */

    return 0;
}