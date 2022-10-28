#ifndef _DATA_DESCRIPTOR_H_
#define _DATA_DESCRIPTOR_H_

#include <stdbool.h>
#include <pipe/pipe.h>

typedef struct dataDescriptor * dataDescriptor_t;
typedef enum  {STD_T, PIPE_T} DATA_TYPE;

dataDescriptor_t create_dataDescriptor(DATA_TYPE type , bool readable, bool writable);
int setPipe_dataDescriptor(dataDescriptor_t dataD ,pipe_t pipe);
pipe_t getPipe_dataDescriptor(dataDescriptor_t dataD);
DATA_TYPE getDataType_dataDescriptor(dataDescriptor_t dataD);
void free_dataDescriptor();

#endif