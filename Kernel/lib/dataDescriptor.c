// This is a personal academic project. Dear PVS-Studio, please check it.
// PVS-Studio Static Code Analyzer for C, C++ and C#: http://www.viva64.com
#include <lib/dataDescriptor.h>
#include <pipe/pipe.h>
#include <pmm.h>
#include <scheduler.h>

typedef struct dataDescriptor {
    DATA_TYPE type;
    mode_t mode;
    pipe_t pipe;
} dataDescriptor;

dataDescriptor_t create_dataDescriptor(DATA_TYPE type, mode_t mode) {

    if (!((mode == READ_MODE || mode == WRITE_MODE) &&
          (type == PIPE_T || type == STD_T)))
        return NULL;

    dataDescriptor_t newDataD = kmalloc(sizeof(dataDescriptor));

    if (newDataD != NULL) {
        newDataD->type = type;
        newDataD->mode = mode;
        newDataD->pipe = NULL;
    }

    return newDataD;
}

DATA_TYPE getDataType_dataDescriptor(dataDescriptor_t dataD) {
    if (dataD == NULL)
        return -1;

    return dataD->type;
}

mode_t getMode_dataDescriptor(dataDescriptor_t dataD) {
    return dataD->mode;
}

pipe_t getPipe_dataDescriptor(dataDescriptor_t dataD) {
    if (dataD == NULL || dataD->type != PIPE_T)
        return NULL;

    return dataD->pipe;
}

int setPipe_dataDescriptor(dataDescriptor_t dataD, pipe_t pipe) {
    if (dataD != NULL && dataD->type == PIPE_T) {
        dataD->pipe = pipe;
        return 0;
    }
    return -1;
}

void close_dataDescriptor(dataDescriptor_t dataD) {
    if (dataD == NULL)
        return;

    if (dataD->type == PIPE_T)
        close_pipe(dataD->pipe, dataD->mode == WRITE_MODE);

    kfree(dataD);
}

int dup2(unsigned int oldfd, unsigned int newfd) {
    process_t *process = get_current_process();

    if (newfd >= process->dataD_index || oldfd >= process->dataD_index)
        return -1;

    dataDescriptor_t aux = process->dataDescriptors[oldfd];
    process->dataDescriptors[oldfd] = process->dataDescriptors[newfd];
    process->dataDescriptors[newfd] = aux;
   
    return 0;
}