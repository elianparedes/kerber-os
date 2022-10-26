#ifndef _SEMAPHORE_H_
#define _SEMAPHORE_H_

#define MAX_SEM_VALUE 10
#define MAX_SEMAPHORES 100

typedef struct sem * sem_ptr;

void init_sem_list();

sem_ptr sem_open(char *, int value);

int sem_wait(sem_ptr semaphore);
 
int sem_post(sem_ptr semaphore);

int sem_close(sem_ptr semaphore);

#endif
