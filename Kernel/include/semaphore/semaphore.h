#ifndef _SEMAPHORE_H_
#define _SEMAPHORE_H_

typedef struct sem_t sem_t;

sem_t sem_open(const char *, int value);

int sem_wait(sem_t*);
 
int sem_post(sem_t*);

int sem_close(sem_t*);

#endif