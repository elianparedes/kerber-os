#ifndef _KPROCESS_H_
#define _KPROCESS_H_

#define MAX_NAME_LENGTH        10
#define PROCESS_TABLE_MAX_SIZE 16

typedef struct process_entry {
    int pid;
    char name[MAX_NAME_LENGTH];
    int priority;
    void *stack;
    void *rbp;
    int children_count;
    char parent_name[MAX_NAME_LENGTH];
    int status;
} process_entry_t;

typedef struct process_table {
    process_entry_t entries[PROCESS_TABLE_MAX_SIZE];
    int count;
} process_table_t;

int _get_process_table(process_table_t *table);

#endif /* _KPROCESS_H_ */