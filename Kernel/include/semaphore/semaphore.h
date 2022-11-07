#ifndef _SEMAPHORE_H_
#define _SEMAPHORE_H_

#define MAX_SEM_VALUE 5
#define MAX_SEMAPHORES 100

typedef struct sem * sem_ptr;
typedef struct copy_sem copy_sem_t;

void init_sem_list();

sem_ptr sem_open(char *, int value);

int sem_wait(sem_ptr semaphore);
 
int sem_post(sem_ptr semaphore);

int sem_close(sem_ptr semaphore);

int get_semaphores(copy_sem_t *sems[]);

#endif
