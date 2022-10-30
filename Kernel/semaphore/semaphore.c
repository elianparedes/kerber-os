#include <semaphore/semaphore.h>
#include <scheduler.h>
#include <pmm.h>
#include <linked_list.h>
#include <lib.h>

#define ERROR -1
#define SUCCESS 0

typedef struct sem
{
    char *name;
    int value;
    int lock;
} sem;

static list_ptr sem_list;
static int sem_count;

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

void init_sem_list()
{
    sem_list = new_linked_list(comparison_function);
}

sem_ptr sem_open(char *name, int value)
{
    sem_ptr sem_find;
    if ((sem_find = find(sem_list, (void *)name, NULL)) == NULL)
    {
        if (sem_count + 1 == MAX_SEMAPHORES){
            return NULL;
        }
        sem_ptr new_sem = kmalloc(sizeof(sem));
        new_sem->name = name;
        new_sem->value = value;
        new_sem->lock = 0;
        add(sem_list, new_sem);
        sem_count++;
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
        //TODO: check
        release(&sem->lock);
        sleep(sem);
    }
    release(&sem->lock);
    return SUCCESS;
}

int sem_post(sem_ptr sem)
{
    acquire(&sem->lock);
    if (sem->value + 1 == MAX_SEM_VALUE){
        release(&sem->lock);
        return ERROR;
    }
    sem->value++;
    wakeup(sem);
    release(&sem->lock);
    return SUCCESS;
}

int sem_close(sem_ptr sem)
{
    if (remove(sem_list, sem) == ERROR){
        return ERROR;
    }
    kfree(sem);
    sem_count--;
    return SUCCESS;
}