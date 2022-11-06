#include <kmman.h>
#include <kprocess.h>
#include <ps.h>

int ps(int argc, char const *argv[]) {

    char *proc_status[] = {"WAITING, READY, TERMINATED"};
    process_table_t *process_table = _malloc(sizeof(process_table_t));

    _get_process_table(process_table);

    for (size_t i = 0; i < process_table->count; i++) {
        printf("pid: %d, status: %d, priority: %d\n",
               process_table->entries[i].pid, process_table->entries[i].status,
               process_table->entries[i].priority);
    }

    return 0;
}
