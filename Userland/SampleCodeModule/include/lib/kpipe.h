#ifndef _KPIPE_H_
#define _KPIPE_H_

typedef struct pipe_info{
    char * name;
    unsigned int nwrite;
    unsigned int nread;
    int readopen;
    int writeopen;
    int blocked_pid[64];
    int blocked_count;
} pipe_info_t;

int _create_pipe(char * name, int fd[2]);
int _open_pipe(char * name, int fd[2]);
int _info_pipe(char * name, pipe_info_t * info);
int _info_all_pipes(pipe_info_t * info[] , unsigned int size);

#endif 