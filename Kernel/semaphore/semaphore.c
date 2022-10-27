#include <semaphore/semaphore.h>
#include <scheduler.h>
#include <pmm.h>
#include <interrupts.h>
#include <fifo_queue.h>
#include <linked_list.h>
#include <lib.h>
#include <graphics.h>

#define MAX_PROCESS 50

typedef struct sem
{
    char *name;
    int value;
    int lock;
    fifo_queue_ptr fifo_queue;
} sem;

static list_ptr sem_list;

extern int _xadd(int *var_ptr, int value);
extern int _xchg(int *var_ptr, int value);

static int comparison_function(void *semaphore, void *name)
{
    sem *sem_test = (sem *)semaphore;
    if (strcmp(sem_test->name, (char *)name) == 0)
    {
        return 1;
    }
    return 0;
}

// spinlock
static void acquire(int *lock)
{
    while (_xchg(lock, 1) != 0);
}

static void release(int *lock)
{
    _xchg(lock, 0);
}

static void sleep(sem_ptr sem)
{
    process_t *current_proc = get_current_process();
    enqueue(sem->fifo_queue, current_proc);
    current_proc->status = PAUSED;
}

static void wakeup(sem_ptr sem)
{
    process_t *process = (process_t *)dequeue(sem->fifo_queue);
    if (process != NULL)
    {
        process->status = READY;
    }
}

void init_sem_list()
{
    sem_list = new_linked_list(comparison_function);
}

sem_ptr sem_open(char *name, int value)
{
    sem_ptr sem_find;
    if ((sem_find = find(sem_list, (void *)name, NULL)) == NULL)
    {
        sem_ptr new_sem = kmalloc(sizeof(sem));
        new_sem->name = name;
        new_sem->value = value;
        new_sem->lock = 0;
        new_sem->fifo_queue = new_fifo_queue();
        return new_sem;
    }
    return sem_find;
}

int sem_wait(sem_ptr sem)
{
    acquire(&sem->lock);
    sem->value--;
    if (sem->value < 0)
    {
        sleep(sem);
        _force_timer_int();
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

int sem_post(sem_ptr sem)
{
    acquire(&sem->lock);
    sem->value++;
    if (sem->value >= 0)
    {
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

int sem_close(sem_ptr sem)
{
    kfree(sem);
    remove(sem_list, sem);
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