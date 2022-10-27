#ifndef _KSEMAPHORE_H_
#define _KSEMAPHORE_H_

typedef struct sem * sem_ptr;

// Semaphore syscalls
sem_ptr _sem_open(char * name, int value);
int _sem_wait(sem_ptr sem);
int _sem_post(sem_ptr sem);
int _sem_close(sem_ptr sem);

#endif
