// This is a personal academic project. Dear PVS-Studio, please check it.
// PVS-Studio Static Code Analyzer for C, C++ and C#: http://www.viva64.com
#include <kmman.h>
#include <ksemaphore.h>
#include <kstdio.h>
#include <printsems.h>

int printsems(int argc, char *argv[]) {

    copy_sem_t *copied_sems[MAX_SEMAPHORES];

    for (int i = 0; i < MAX_SEMAPHORES; i++) {
        copied_sems[i] = (copy_sem_t *)_malloc(sizeof(copy_sem_t));
        for (int k = 0; k < MAX_BLOCKED_PROCESSES; k++) {
            copied_sems[i]->blocked_processes[k] = 0;
        }
    }

    int i = _get_semaphores(copied_sems);
    printf("NAME           |  VALUE  | BLOCKED PIDS      \n");
    printf("\n");
    copy_sem_t *aux_sem;
    for (int j = 0; j < i; j++) {
        aux_sem = copied_sems[j];
        printf("%15s|", aux_sem->name);
        printf("  %7d|", aux_sem->value);
        for (int k = 0; aux_sem->blocked_processes[k] > 0; k++) {
            printf(" %d", aux_sem->blocked_processes[k]);
        }
        printf("\n");
    }

    for (int k = 0; k < MAX_SEMAPHORES; k++) {
        _free(copied_sems[k]);
    }

    return 0;
}