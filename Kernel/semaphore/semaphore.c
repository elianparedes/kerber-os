// This is a personal academic project. Dear PVS-Studio, please check it.
// PVS-Studio Static Code Analyzer for C, C++ and C#: http://www.viva64.com
#include <semaphore/semaphore.h>
#include <scheduler.h>
#include <pmm.h>
#include <linked_list.h>
#include <lib.h>

#define ERROR -1
#define SUCCESS 0

typedef struct sem
{
    char name[40];
    int value;
    int lock;
    list_ptr blocked_processes;
} sem_t;

typedef struct copy_sem
{
    char name[40];
    int value;
    int lock;
    int blocked_processes[];
} copy_sem_t;

static list_ptr sem_list;
static int sem_count;

extern int _xadd(int *var_ptr, int value);
extern int _xchg(int *var_ptr, int value);

static int comparison_function(void *semaphore, void *name)
{
    sem_t *sem_test = (sem_t *)semaphore;
    if (strcmp(sem_test->name, (char *)name) == 0)
    {
        return 1;
    }
    return 0;
}

static int process_comparison_function(void * pid, void * other_pid){
    return (*(int *)pid) == (*(int *)other_pid);
}

// spinlock
static void acquire(int *lock) {
    while (_xchg(lock, 1) != 0)
        ;
}

static void release(int *lock) {
    _xchg(lock, 0);
}

void init_sem_list()
{
    sem_list = new_linked_list(comparison_function);
}

sem_ptr sem_open(char *name, int value) {
    sem_ptr sem_find;
    if ((sem_find = find(sem_list, (void *)name, NULL)) == NULL)
    {
        if (sem_count + 1 == MAX_SEMAPHORES){
            return NULL;
        }
        sem_ptr new_sem = kmalloc(sizeof(sem_t));
        strcpy(new_sem->name, name);
        new_sem->value = value;
        new_sem->lock = 0;
        new_sem->blocked_processes=new_linked_list(process_comparison_function);
        add(sem_list, new_sem);
        sem_count++;
        return new_sem;
    }
    return sem_find;
}

int sem_wait(sem_ptr sem) {
    acquire(&sem->lock);
    sem->value--;
    if (sem->value < 0)
    {
        //TODO: check
        process_t *current_process = get_current_process();
        add(sem->blocked_processes, (void *)&current_process->pid);
        release(&sem->lock);
        sleep((uint64_t)sem);
    }
    release(&sem->lock);
    return SUCCESS;
}

int sem_post(sem_ptr sem) {
    acquire(&sem->lock);
    if (sem->value + 1 == MAX_SEM_VALUE){
        release(&sem->lock);
        return ERROR;
    }
    sem->value++;
    int pid = wakeup((uint64_t)sem);
    if (pid != ERROR){
        remove(sem->blocked_processes, (void *)&pid);
        release(&sem->lock);
        return SUCCESS;
    }
    release(&sem->lock);
    return ERROR;
}

int sem_close(sem_ptr sem)
{
    if (remove(sem_list, sem) == ERROR){
        return ERROR;
    }
    free_list(sem->blocked_processes);
    kfree(sem);
    sem_count--;
    return SUCCESS;
}

static int get_blocked_processes(sem_t sem, int blocked_processes[]){
    to_begin(sem.blocked_processes);
    int i=0;
    while (hasNext(sem.blocked_processes)){
        blocked_processes[i++]=(*(int *)next(sem.blocked_processes));
    }
    return i;
}

int get_semaphores(copy_sem_t * sems[]){
    int i=0;
    to_begin(sem_list);
    while (hasNext(sem_list)){
        sem_t aux_sem=*((sem_ptr)next(sem_list));
        strcpy(sems[i]->name, aux_sem.name);
        sems[i]->value=aux_sem.value;
        sems[i]->lock=aux_sem.lock;
        get_blocked_processes(aux_sem, sems[i]->blocked_processes);
        i++;
    }
    return i;
}

