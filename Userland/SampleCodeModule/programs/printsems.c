#include <kmman.h>
#include <ksemaphore.h>
#include <kstdio.h>
#include <printsems.h>

#define ERROR -1

sem_ptr sem1;
sem_ptr sem2;

int dummy_process() {
    _sem_wait(sem1);
    return 0;
}

void printsems() {

    copy_sem_t *copied_sems[MAX_SEMAPHORES];

    sem1 = _sem_open("dummy_sem1", 0);
    sem2 = _sem_open("dummy_sem2", 1);
    int pid = _run(dummy_process, NULL, NULL);
    _sched_yield();

    for (int i = 0; i < MAX_SEMAPHORES; i++) {
        copied_sems[i] = (copy_sem_t *)_malloc(sizeof(copy_sem_t));
        for (int k=0; k < MAX_BLOCKED_PROCESSES; k++){
            copied_sems[i]->blocked_processes[k]=0;
        }
    }

    int i = _get_semaphores(copied_sems);
    _sem_post(sem1);
    _wait();
    copy_sem_t *aux_sem;
    for (int j = 0; j < i; j++) {
        aux_sem = copied_sems[j];
        printf("Name: %s |", aux_sem->name);
        printf(" Value: %d |", aux_sem->value);
        printf(" Blocked pids:");
        for (int k = 0; aux_sem->blocked_processes[k] > 0; k++) {
            printf(" %d", aux_sem->blocked_processes[k]);
        }
        printf("| \n");
    }

    for (int k = 0; k < MAX_SEMAPHORES; k++) {
        _free(copied_sems[k]);
    }

    if (_sem_close(sem1) == ERROR) {
        printf("Error closing semaphore \n");
    };

    if (_sem_close(sem2) == ERROR) {
        printf("Error closing semaphore \n");
    }
}