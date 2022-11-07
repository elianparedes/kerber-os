#include <kmman.h>
#include <kprocess.h>
#include <kstdio.h>
#include <ps.h>

static void draw_header() {
    printf("%4s %12s %8s %10s %10s %5s %10s %8s\n", "PID", "NAME", "STATE",
           "PRIORITY", "PARENT", "CHLD", "STACK", "BP");
}

int ps(int argc, char const *argv[]) {

    char *status[] = {"WAITING", "READY", "TERM"};
    process_table_t *process_table = _malloc(sizeof(process_table_t));

    _get_process_table(process_table);

    draw_header();
    for (size_t i = 0; i < process_table->count; i++) {
        printf("%4d %12s %8s %10d %10s %5d 0x%8x 0x%8x\n",
               process_table->entries[i].pid, process_table->entries[i].name,
               status[process_table->entries[i].status],
               process_table->entries[i].priority,
               process_table->entries[i].parent_name,
               process_table->entries[i].children_count,
               process_table->entries[i].stack, process_table->entries[i].rbp);
    }

    _free(process_table);

    return 0;
}
