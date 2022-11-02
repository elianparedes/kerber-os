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
    list_ptr blocked_pid;
} pipe;

typedef struct pipe_info{
    char * name;
    unsigned int nwrite;
    unsigned int nread;
    int readopen;
    int writeopen;
    int blocked_pid[64];
    int blocked_count;
} pipe_info;

list_ptr pipe_list;

static int comparison_function(void *pipe, void *name)
{
    pipe_t pipe_test = (pipe_t)pipe;
    char *name_test = (char *)name;

    return !strcmp(pipe_test->name, name_test);
}

static int process_comparison_function(void * pid, void * other_pid){
    return (*(int *)pid) == (*(int *)other_pid);
}

static void wakeup_helper(uint64_t channel, list_ptr blocked_list){
    int pid = wakeup(channel);
    remove(blocked_list,(void *)&pid);
}

static void sleep_helper(uint64_t channel,list_ptr blocked_list){
    process_t * process = get_current_process();
    add(blocked_list,&process->pid);
    sleep(channel);
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
    newPipe->blocked_pid = new_linked_list(process_comparison_function);

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
            wakeup_helper((uint64_t)&pipe->nread,pipe->blocked_pid);
            sleep_helper((uint64_t)&pipe->nwrite,pipe->blocked_pid);
            //wakeup((uint64_t)&pipe->nread);
            //sleep((uint64_t)&pipe->nwrite);
            
        }
        pipe->data[pipe->nwrite++ % PIPE_SIZE] = buffer[i];
    }
    wakeup_helper((uint64_t)&pipe->nread,pipe->blocked_pid);
    //wakeup((uint64_t)&pipe->nread);
    return i;
}

int piperead(pipe_t pipe, char *buffer, int count)
{

    if (pipe == NULL || buffer == NULL || count < 0)
        return -1;

    while(pipe->nread == pipe->nwrite){
        sleep_helper((uint64_t)&pipe->nread,pipe->blocked_pid);
        //sleep((uint64_t)&pipe->nread);
    }  

    int i;
    for (i = 0; i < count; i++){
        if(pipe->nread == pipe->nwrite)
            break;
        buffer[i] = pipe->data[pipe->nread++ % PIPE_SIZE];
    }
    wakeup_helper((uint64_t)&pipe->nwrite,pipe->blocked_pid);
    //wakeup((uint64_t)&pipe->nwrite);

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
        kfree(pipe->blocked_pid);
        kfree(pipe);
    }

    to_begin(pipe_list);

    if (!hasNext(pipe_list)){
        kfree(pipe_list);
        pipe_list = NULL;
    }

    return;
}

static int copy_pids(list_ptr blocked_pid , int blocked_pid_cpy[] ){

    to_begin(blocked_pid);

    int count = 0;

    while(hasNext(blocked_pid)){
        int pid = *(int *)next(blocked_pid);
        blocked_pid_cpy[count++] = pid;
    }

    return count;
}

static void copy_info(pipe_t pipe , pipe_info_t * info){

    info->name = pipe->name;
    info->nread = pipe->nread;
    info->nwrite = pipe->nwrite;
    info->readopen = pipe->readopen;
    info->writeopen = pipe->writeopen;

    info->blocked_count = copy_pids(pipe->blocked_pid,info->blocked_pid);
}

int info_pipe(char * name, pipe_info_t * info){

    pipe_t pipe = find(pipe_list,name,NULL);
    if(pipe == NULL)
        return -1;

    copy_info(pipe,info);
    
    return 0;
}

int info_all_pipes( pipe_info_t * info_arr[] , unsigned int size){

    int count = 0;

    if(pipe_list == NULL)
        return 0;

    to_begin(pipe_list);

    while (hasNext(pipe_list) && count < size ){
        copy_info((pipe_t)next(pipe_list),info_arr[count++]);
    }

    return count;
    
}