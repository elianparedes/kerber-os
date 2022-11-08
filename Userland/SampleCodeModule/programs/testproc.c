#include <kstdio.h>
#include <test_util.h>

typedef struct P_rq {
    int pid;
    pstatus_t state;
} p_rq;

int test_proc(int argc, char *argv[]) {
    uint8_t rq;
    uint8_t alive = 0;
    uint8_t action;
    uint64_t max_processes;
    char *argvAux[] = {"endless"};

    if (argc != 2)
        return -1;

    if ((max_processes = satoi(argv[1])) <= 0)
        return -1;

    p_rq p_rqs[max_processes];

    while (1) {

        // Create max_processes processes
        for (rq = 0; rq < max_processes; rq++) {
            p_rqs[rq].pid = _run(endless_loop, 1, argvAux);

            if (p_rqs[rq].pid == -1) {
                printf("test_processes: ERROR creating process\n");
                return -1;
            } else {
                p_rqs[rq].state = READY;
                alive++;
            }
        }

        // Randomly kills, blocks or unblocks processes until every one has been
        // killed
        while (alive > 0) {

            for (rq = 0; rq < max_processes; rq++) {
                action = GetUniform(100) % 2;

                switch (action) {
                    case 0:
                        if (p_rqs[rq].state == READY ||
                            p_rqs[rq].state == WAITING) {
                            if (_kill(p_rqs[rq].pid) == -1) {
                                printf(
                                    "test_processes: ERROR killing process\n");
                                return -1;
                            }
                            _wait();
                            p_rqs[rq].state = TERMINATED;
                            alive--;
                        }
                        break;

                    case 1:
                        if (p_rqs[rq].state == READY) {
                            if (_block(p_rqs[rq].pid) == -1) {
                                printf(
                                    "test_processes: ERROR blocking process\n");
                                return -1;
                            }
                            p_rqs[rq].state = WAITING;
                        }
                        break;
                }
            }

            // Randomly unblocks processes
            for (rq = 0; rq < max_processes; rq++)
                if (p_rqs[rq].state == WAITING && GetUniform(100) % 2) {
                    if (_unblock(p_rqs[rq].pid) == -1) {
                        printf("test_processes: ERROR unblocking process\n");
                        return -1;
                    }
                    p_rqs[rq].state = READY;
                }
        }
    }
}