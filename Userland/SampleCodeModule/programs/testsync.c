#include <ksemaphore.h>
#include <kstdio.h>
#include <test_util.h>

#define SEM_ID               "sem"
#define TOTAL_PAIR_PROCESSES 2

int64_t global = 0; // shared memory

void slowInc(int64_t *p, int inc) {
    uint64_t aux = *p;
    _sched_yield(); // This makes the race condition highly probable
    aux += inc;
    *p = aux;
}

int myprocinc(int argc, char *argv[]) {
    uint64_t n;
    int8_t inc;
    int8_t use_sem;
    sem_ptr sem;

    if (argc != 4)
        return -1;

    if ((n = satoi(argv[1])) <= 0)
        return -1;
    if ((inc = satoi(argv[2])) == 0)
        return -1;
    if ((use_sem = satoi(argv[3])) < 0)
        return -1;

    if (use_sem)
        if (!(sem = _sem_open(SEM_ID, 1))) {
            printf("test_sync: error opening semaphore\n");
            return -1;
        }

    uint64_t i;
    for (i = 0; i < n; i++) {
        if (use_sem)
            _sem_wait(sem);
        slowInc(&global, inc);
        if (use_sem)
            _sem_post(sem);
    }

    if (use_sem)
        _sem_close(sem);

    return 0;
}

int test_sync(int argc, char *argv[]) { //{n, use_sem, 0}

    if (argc < 3) {
        printf("test_sync: missing arguments\n");
        return -1;
    }

    if (argc > 3) {
        printf("test_sync: too many arguments\n");
        return -1;
    }

    if (satoi(argv[1]) <= 0) {
        printf("test_sync: increment is not a valid argument\n");
        return -1;
    }

    if (satoi(argv[2]) < 0) {
        printf("test_sync: use_sem is not a valid argument\n");
        return -1;
    }

    int exit_status;
    int pids[2 * TOTAL_PAIR_PROCESSES];
    global = 0;

    char *argvDec[] = {"myprocinc", argv[1], "-1", argv[2]};
    char *argvInc[] = {"myprocinc", argv[1], "1", argv[2]};

    uint64_t i;
    while (1) {
        for (i = 0; i < TOTAL_PAIR_PROCESSES; i++) {
            pids[i] = _run(myprocinc, 4, argvDec);
            pids[i + TOTAL_PAIR_PROCESSES] = _run(myprocinc, 4, argvInc);
        }

        for (i = 0; i < TOTAL_PAIR_PROCESSES; i++) {
            _waitpid(pids[i], &exit_status);
            _waitpid(pids[i + TOTAL_PAIR_PROCESSES], &exit_status);
        }

        // printf("Final value: %d\n", global);
    }

    return 0;
}