#include <ksemaphore.h>
#include <kstdio.h>
#include <test_util.h>
#include <testsync.h>

// From alejoaquili ITBA-72.11-SO repository

#define SEM_ID               "sem"
#define TOTAL_PAIR_PROCESSES 2

int64_t global; // shared memory
int8_t use_sem;
sem_ptr sem;

void slowInc(int64_t *p, int64_t inc) {
    uint64_t aux = *p;
    _sched_yield();
    aux += inc;
    *p = aux;
}

uint64_t my_process_inc(char *increment) {
    printf("Nuevo proceso creado \n");
    uint64_t n = 100;
    int8_t inc = satoi(increment);

    if (use_sem) {
        if (!(sem = _sem_open(SEM_ID, 1))) {
            printf("test_sync: ERROR opening semaphore\n");
            return -1;
        }
    } else {
        printf("No sem\n");
    }
    uint64_t i;
    for (i = 0; i < n; i++) {
        if (use_sem) {
            _sem_wait(sem);
        }
        slowInc(&global, inc);
        printf("%d \n", global);
        if (use_sem) {
            _sem_post(sem);
        }
    }

    if (use_sem)
        _sem_close(sem);

    printf("Termino un proceso \n");
    return 0;
}

//{n, use_sem, 0}
uint64_t test_sync(int argc, char *argv[]) {
    uint64_t pids[2 * TOTAL_PAIR_PROCESSES];

    global = 0;

    use_sem = satoi(argv[0]);

    printf("%d \n", use_sem);
    uint64_t i;
    for (i = 0; i < TOTAL_PAIR_PROCESSES; i++) {
        char arg1[1] = {"1"};
        char arg2[1] = {"-1"};
        pids[i] = _run(my_process_inc, 1, arg1);
        pids[i + TOTAL_PAIR_PROCESSES] = _run(my_process_inc, 1, arg2);
    }

    while (1) {
    }

    return 0;
}