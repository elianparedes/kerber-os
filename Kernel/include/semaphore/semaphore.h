#ifndef _SEMAPHORE_H_
#define _SEMAPHORE_H_

typedef struct sem * sem_ptr;

sem_ptr sem_open(const char *, int value);

int sem_wait(sem_ptr semaphore);
 
int sem_post(sem_ptr semaphore);

int sem_close(sem_ptr semaphore);

#endif