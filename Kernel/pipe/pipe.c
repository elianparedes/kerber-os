#include <pipe/pipe.h>
#include <linked_list.h>
#include <lib/dataDescriptor.h>
#include <lib.h>
#include <pmm.h>
#include <scheduler.h>

#define PIPE_SIZE (1024)

typedef struct pipe {
    char * name;
    char data[PIPE_SIZE];
    int nwrite;
    int nread;
    int readopen;
    int writeopen;
} pipe;


list_t pipe_list;

static int comparison_function(void * pipe , void * name){
    pipe_t pipe_test = (pipe_t) pipe;
    char * name_test = (char *) name;

    return !strcmp(pipe_test->name,name_test);
}

static void set_data_descriptors(pipe_t pipe, int pipe_dataD[2]){

    dataDescriptor_t readEnd = create_dataDescriptor(PIPE_T,READ_MODE);
    setPipe_dataDescriptor(readEnd,pipe);
    pipe->readopen++;

    dataDescriptor_t writeEnd = create_dataDescriptor(PIPE_T,WRITE_MODE);
    setPipe_dataDescriptor(writeEnd,pipe);
    pipe->writeopen++;

    /**
     * TODO: Considerar si el agregado a procesos debe hacerse en la syscall y que el pipe devuelva en un array los punteros a dataD 
     */
    process_t * process = get_current_process();
    pipe_dataD[0] = process->dataD_index;
    process->dataDescriptors[process->dataD_index++] = readEnd;
    pipe_dataD[1] = process->dataD_index;
    process->dataDescriptors[process->dataD_index++] = writeEnd;
}

int init_pipes(){
    if(pipe_list != NULL)
        return -1;

    pipe_list = new_linked_list(comparison_function);
    return 0;
}

int create_pipe(char * name, int pipe_dataD[2]){

    if(pipe_list == NULL)
        init_pipes();

    if(find(pipe_list,name) != NULL){
        return -1;
    }

    pipe_t newPipe = kmalloc(sizeof(pipe));
    if(newPipe == NULL)
        return -1;

    newPipe->name = name;
    newPipe->nread = 0;
    newPipe->nwrite = 0;
    newPipe->readopen = 0;
    newPipe->writeopen = 0;

    add(pipe_list,newPipe);

    set_data_descriptors(newPipe,pipe_dataD);

    return 0;
}

int open_pipe(char * name , int pipe_dataD[2]){

    if(pipe_list == NULL)
        return -1;

    pipe_t pipe = find(pipe_list,name);

    if( pipe != NULL){
        return -1;
    }

    set_data_descriptors(pipe,pipe_dataD);

    return 0;

}

int pipewrite(pipe_t pipe, const char * buffer, int count){

    if(pipe == NULL || buffer == NULL || count < 0)
        return -1;

    int i;
    for(i = 0 ; i < count ; i++){
        pipe->data[pipe->nwrite++] = buffer[i];
    }
    return i;
}

int piperead(pipe_t pipe, char * buffer, int count){

    if(pipe == NULL || buffer == NULL || count < 0)
        return -1;

    int i;
    for(i = 0 ; i < count ; i++){
        buffer[i] = pipe->data[pipe->nread++]; 
    }

    return i;
}

void close_pipe(pipe_t pipe, mode_t mode){

    if(pipe_list == NULL || pipe == NULL)
        return;
    
    switch (mode)
    {
    case READ_MODE:
        if(pipe->readopen != 0){
            pipe->readopen--;
        }
        break;

    case WRITE_MODE:
    if(pipe->writeopen != 0){
        pipe->writeopen--;
    }
    break;
    
    default:
        return;
    }
    
    if(pipe->readopen == 0 && pipe->writeopen == 0){
        remove(pipe_list,pipe);
        kfree(pipe);
    }

    to_begin(pipe_list);

    if(!hasNext(pipe_list)){
        kfree(pipe_list);
        pipe_list = NULL;
    }

    return;
}