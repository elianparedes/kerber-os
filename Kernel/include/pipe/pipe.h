#ifndef _PIPE_H_
#define _PIPE_H_

#include <stdlib.h>

typedef struct pipe * pipe_t;

int init_pipes();
int create_pipe(char * name, int pipe_dataD[2]);
int open_pipe(char * name , int pipe_dataD[2]);
int piperead(pipe_t pipe, char * buffer, int count);
int pipewrite(pipe_t pipe, const char * buffer, int count);
void pipe_free();

#endif
