#ifndef _DATA_DESCRIPTOR_H_
#define _DATA_DESCRIPTOR_H_

#include <stdbool.h>
#include <pipe/pipe.h>

typedef struct dataDescriptor * dataDescriptor_t;
typedef enum  {STD_T, PIPE_T} DATA_TYPE;
typedef enum  {READ_MODE, WRITE_MODE} mode_t;

/**
 * @retval  NULL if the specified type or mode is wrong
 */
dataDescriptor_t create_dataDescriptor(DATA_TYPE type , mode_t mode);

/** 
 * @retval 0 on success. -1 if the dataD is not of PIPE_T type
 */
int setPipe_dataDescriptor(dataDescriptor_t dataD ,pipe_t pipe);

/** 
 * @retval NULL if an error ocurred
 */
pipe_t getPipe_dataDescriptor(dataDescriptor_t dataD);

/** 
 * @retval -1 if an error ocurred
 */
DATA_TYPE getDataType_dataDescriptor(dataDescriptor_t dataD);

mode_t getMode_dataDescriptor(dataDescriptor_t dataD);

void close_dataDescriptor(dataDescriptor_t dataD);

int dup2(unsigned int oldfd , unsigned int newfd);

#endif