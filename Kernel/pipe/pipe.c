#include <pipe/pipe.h>
#include <linked_list.h>
#include <lib.h>
#include <pmm.h>
#include <scheduler.h>

#define PIPE_SIZE (1024)

typedef struct pipe
{
    char *name;
    char data[PIPE_SIZE];
    unsigned int nwrite;
    unsigned int nread;
    int readopen;
    int writeopen;
} pipe;

list_ptr pipe_list;

static int comparison_function(void *pipe, void *name)
{
    pipe_t pipe_test = (pipe_t)pipe;
    char *name_test = (char *)name;

    return !strcmp(pipe_test->name, name_test);
}

static void set_data_descriptors(pipe_t pipe, int pipe_dataD[2])
{

    dataDescriptor_t readEnd = create_dataDescriptor(PIPE_T, READ_MODE);
    setPipe_dataDescriptor(readEnd, pipe);
    pipe->readopen++;

    dataDescriptor_t writeEnd = create_dataDescriptor(PIPE_T, WRITE_MODE);
    setPipe_dataDescriptor(writeEnd, pipe);
    pipe->writeopen++;

    /**
     * TODO: Considerar si el agregado a procesos debe hacerse en la syscall y que el pipe devuelva en un array los punteros a dataD
     */
    process_t *process = get_current_process();
    pipe_dataD[0] = process->dataD_index;
    process->dataDescriptors[process->dataD_index++] = readEnd;
    pipe_dataD[1] = process->dataD_index;
    process->dataDescriptors[process->dataD_index++] = writeEnd;
}

int init_pipes()
{
    pipe_list = new_linked_list(comparison_function);
    return 0;
}

int create_pipe(char *name, int pipe_dataD[2])
{

    if (pipe_list == NULL)
        init_pipes();

    if (find(pipe_list, name,NULL) != NULL)
    {
        return -1;
    }

    pipe_t newPipe = kmalloc(sizeof(pipe));
    if (newPipe == NULL)
        return -1;

    newPipe->name = name;
    newPipe->nread = 0;
    newPipe->nwrite = 0;
    newPipe->readopen = 0;
    newPipe->writeopen = 0;

    add(pipe_list, newPipe);

    set_data_descriptors(newPipe, pipe_dataD);

    return 0;
}

int open_pipe(char *name, int pipe_dataD[2])
{

    if (pipe_list == NULL)
        return -1;

    pipe_t pipe = find(pipe_list, name,NULL);

    if (pipe == NULL)
    {
        return -1;
    }

    set_data_descriptors(pipe, pipe_dataD);

    return 0;
}

int pipewrite(pipe_t pipe, const char *buffer, int count)
{

    if (pipe == NULL || buffer == NULL || count < 0)
        return -1;

    int i;
    for (i = 0; i < count; i++)
    {
        while(pipe->nwrite == pipe->nread + PIPE_SIZE){
            wakeup((uint64_t)&pipe->nread);
            sleep((uint64_t)&pipe->nwrite);
        }
        pipe->data[pipe->nwrite++ % PIPE_SIZE] = buffer[i];
    }
    wakeup((uint64_t)&pipe->nread);
    return i;
}

int piperead(pipe_t pipe, char *buffer, int count)
{

    if (pipe == NULL || buffer == NULL || count < 0)
        return -1;

    while(pipe->nread == pipe->nwrite){
        sleep((uint64_t)&pipe->nread);
    }  

    int i;
    for (i = 0; i < count; i++){
        if(pipe->nread == pipe->nwrite)
            break;
        buffer[i] = pipe->data[pipe->nread++ % PIPE_SIZE];
    }

    wakeup((uint64_t)&pipe->nwrite);

    return i;
}

void close_pipe(pipe_t pipe, int writable)
{

    if (pipe_list == NULL || pipe == NULL)
        return;

    if (writable){
        if (pipe->writeopen != 0){
            pipe->writeopen--;
        }
    }
    else{
        if (pipe->readopen != 0){
            pipe->readopen--;
        }
    }

    if (pipe->readopen == 0 && pipe->writeopen == 0 && pipe->nread > 0 && pipe->nwrite > 0){
        remove(pipe_list, pipe->name);
        kfree(pipe);
    }

    to_begin(pipe_list);

    if (!hasNext(pipe_list)){
        kfree(pipe_list);
        pipe_list = NULL;
    }

    return;
}