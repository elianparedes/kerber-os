#ifndef _KSEMAPHORE_H_
#define _KSEMAPHORE_H_

#define MAX_SEMAPHORES 15
#define MAX_BLOCKED_PROCESSES 10

typedef struct sem * sem_ptr;

typedef struct copy_sem
{
    char name[40];
    int value;
    int lock;
    int blocked_processes[MAX_BLOCKED_PROCESSES];
} copy_sem_t;

// Semaphore syscalls
sem_ptr _sem_open(char * name, int value);

int _sem_wait(sem_ptr sem);

int _sem_post(sem_ptr sem);

int _sem_close(sem_ptr sem);

int _get_semaphores(copy_sem_t * sems[]);

#endif
