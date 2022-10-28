#include <lib/dataDescriptor.h>
#include <pmm.h>

typedef struct dataDescriptor{
    DATA_TYPE type;
    bool readable;
    bool writable;
    pipe_t pipe;
} dataDescriptor;

dataDescriptor_t create_dataDescriptor(DATA_TYPE type , bool readable, bool writable){
    dataDescriptor_t newDataD = kmalloc(sizeof(dataDescriptor));

    newDataD->type = type;
    newDataD->readable = readable;
    newDataD->writable = writable;
    newDataD->pipe = NULL;

    return newDataD;
}

DATA_TYPE getDataType_dataDescriptor(dataDescriptor_t dataD){
    return dataD->type;
}

pipe_t getPipe_dataDescriptor(dataDescriptor_t dataD){
    return dataD->pipe;
}

int setPipe_dataDescriptor(dataDescriptor_t dataD ,pipe_t pipe){
    if(dataD->type == PIPE_T){
        dataD->pipe = pipe;
        return 0;
    }
    return -1;
}