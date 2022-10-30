#include <lib/dataDescriptor.h>
#include <pmm.h>

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

    if(dataD->mode == PIPE_T)
        close_pipe(dataD->pipe,dataD->mode);
    
    kfree(dataD);
}