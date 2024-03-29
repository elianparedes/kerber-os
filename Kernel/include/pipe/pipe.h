#ifndef _PIPE_H_
#define _PIPE_H_

#include <stdlib.h>

typedef struct pipe * pipe_t;
typedef struct pipe_info pipe_info_t;

/**   
 * @retval 0 on success. -1 if pipe have been already initialized 
 */
int init_pipes();

/**   
 * @retval 0 on success. -1 if the pipe have been already created or the pipe can´t be created
 */
int create_pipe(char * name, int pipe_dataD[2]);

/**   
 * @retval 0 on success. -1 if pipes have not been initialized or there is not pipe with the name given
 */
int open_pipe(char * name , int pipe_dataD[2]);

/**   
 * @retval the number of bytes readed on success. -1 in error
 */
int piperead(pipe_t pipe, char * buffer, int count);

int pipewrite(pipe_t pipe, const char * buffer, int count);

void close_pipe(pipe_t pipe, int writable);

int info_pipe(char * name, pipe_info_t * info);

int info_all_pipes( pipe_info_t * info_arr[], unsigned int size);

void add_writer(pipe_t pipe);

void add_reader(pipe_t pipe);

#endif
