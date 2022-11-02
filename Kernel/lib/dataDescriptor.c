#include <lib/dataDescriptor.h>
#include <pmm.h>
#include <scheduler.h>

typedef struct dataDescriptor{
    DATA_TYPE type;
    mode_t mode;
    pipe_t pipe;
} dataDescriptor;

dataDescriptor_t create_dataDescriptor(DATA_TYPE type , mode_t mode){

    if(!((mode == READ_MODE || mode == WRITE_MODE) && (type == PIPE_T || type == STD_T )))
        return NULL;
    
    dataDescriptor_t newDataD = kmalloc(sizeof(dataDescriptor));
    
    if(newDataD != NULL){
        newDataD->type = type;
        newDataD->mode = mode;
        newDataD->pipe = NULL;
    }

    return newDataD;
}

DATA_TYPE getDataType_dataDescriptor(dataDescriptor_t dataD){
    if(dataD == NULL)
        return -1;

    return dataD->type;
}

mode_t getMode_dataDescriptor(dataDescriptor_t dataD){
    return dataD->mode;
}

pipe_t getPipe_dataDescriptor(dataDescriptor_t dataD){
    if( dataD == NULL || dataD->type != PIPE_T)
        return NULL;

    return dataD->pipe;
}

int setPipe_dataDescriptor(dataDescriptor_t dataD ,pipe_t pipe){
    if(dataD != NULL && dataD->type == PIPE_T){
        dataD->pipe = pipe;
        return 0;
    }
    return -1;
}

void close_dataDescriptor(dataDescriptor_t dataD){
    if(dataD == NULL)
        return;

    if(dataD->type == PIPE_T)
        close_pipe(dataD->pipe,dataD->mode == WRITE_MODE);
    
    kfree(dataD);
}

int dup2(unsigned int oldfd , unsigned int newfd){
    process_t * process = get_current_process();

    if(newfd >= process->dataD_index || oldfd >= process->dataD_index)
        return -1;

    close_dataDescriptor(process->dataDescriptors[newfd]);

    dataDescriptor_t dataD = process->dataDescriptors[oldfd];
    mode_t mode = getMode_dataDescriptor(dataD);
    DATA_TYPE type = getDataType_dataDescriptor(dataD);
    pipe_t pipe = getPipe_dataDescriptor(dataD);

    dataDescriptor_t dataD_cpy = create_dataDescriptor(type,mode);
    setPipe_dataDescriptor(dataD_cpy,pipe);

    process->dataDescriptors[newfd] = dataD_cpy;

    return 0;
}