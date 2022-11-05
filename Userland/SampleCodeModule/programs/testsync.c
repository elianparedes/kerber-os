#include <kstdio.h>
#include <test_util.h>
#include <ksemaphore.h>

#define SEM_ID "sem"
#define TOTAL_PAIR_PROCESSES 2

int64_t global; // shared memory

void slowInc(int64_t *p, int64_t inc)
{
    uint64_t aux = *p;
    _sched_yield(); // This makes the race condition highly probable
    aux += inc;
    *p = aux;
}

uint64_t my_process_inc(uint64_t argc, char *argv[])
{
    uint64_t n;
    int8_t inc;
    int8_t use_sem;

    if (argc != 3)
        return -1;

    if ((n = satoi(argv[0])) <= 0)
        return -1;
    if ((inc = satoi(argv[1])) == 0)
        return -1;
    if ((use_sem = satoi(argv[2])) < 0)
        return -1;

    printf("Entre aca \n");

    sem_ptr sem;
    if (use_sem)
        if (!(sem=_sem_open(SEM_ID, 1)))
        {
            printf("test_sync: ERROR opening semaphore\n");
            return -1;
        }

    uint64_t i;
    for (i = 0; i < n; i++)
    {
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

uint64_t test_sync(uint64_t argc, char *argv[])
{ //{n, use_sem, 0}
    uint64_t pids[2 * TOTAL_PAIR_PROCESSES];

    if (argc != 2)
        return -1;

    printf("arg0: %s \n", argv[0]);
    printf("arg1: %s \n", argv[1]);
    char *argvDec[] = {argv[0], "-1", argv[1], NULL};
    char *argvInc[] = {argv[0], "1", argv[1], NULL};

    global = 0;

    uint64_t i;
    for (i = 0; i < TOTAL_PAIR_PROCESSES; i++)
    {
        pids[i] = _run(my_process_inc, 3, argvDec);
        pids[i + TOTAL_PAIR_PROCESSES] = _run(my_process_inc, 3, argvInc);
    }

    for (i = 0; i < TOTAL_PAIR_PROCESSES; i++)
    {
        _wait();
        _wait();
        //my_wait(pids[i]);
        //my_wait(pids[i + TOTAL_PAIR_PROCESSES]);
    }

    printf("Final value: %d\n", global);

    return 0;
}