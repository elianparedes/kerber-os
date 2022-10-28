#include <pipe/pipe.h>
#include <linked_list.h>
#include <lib/dataDescriptor.h>
#include <lib.h>
#include <pmm.h>
#include <scheduler.h>

#define PIPE_SIZE (1024)

int pipe_counter = 0;

typedef struct pipe {
    char * name;
    char data[PIPE_SIZE];
    int nwrite;
    int nread;
} pipe;


list_t pipe_list;

static int comparison_function(void * pipe , void * name){
    pipe_t pipe_test = (pipe_t) pipe;
    char * name_test = (char *) name;

    return !strcmp(pipe_test->name,name_test);
}

static void set_data_descriptors(pipe_t pipe, int pipe_dataD[2]){

    dataDescriptor_t readEnd = create_dataDescriptor(PIPE_T,true, false);
    setPipe_dataDescriptor(readEnd,pipe);
    dataDescriptor_t writeEnd = create_dataDescriptor(PIPE_T,false, true);
    setPipe_dataDescriptor(writeEnd,pipe);

    process_t * process = get_current_process();
    pipe_dataD[0] = process->dataD_index;
    process->dataDescriptors[process->dataD_index++] = readEnd;
    pipe_dataD[1] = process->dataD_index;
    process->dataDescriptors[process->dataD_index++] = writeEnd;
}

int init_pipes(){
    pipe_list = new_linked_list(comparison_function);
    return 0;
}

int create_pipe(char * name, int pipe_dataD[2]){

    if(find(pipe_list,name) != NULL){
        return -1;
    }

    pipe_t pipe = kmalloc(sizeof(pipe));
    pipe->name = name;
    pipe->nread = 0;
    pipe->nwrite = 0;

    add(pipe_list,(void *)pipe);

    set_data_descriptors(pipe,pipe_dataD);

    return 0;
}

int open_pipe(char * name , int pipe_dataD[2]){

    pipe_t pipe = find(pipe_list,name);

    if( pipe == NULL){
        return -1;
    }

    set_data_descriptors(pipe,pipe_dataD);

    return 0;

}

int pipewrite(pipe_t pipe, const char * buffer, int count){
    int i;
    int nwrite = pipe->nwrite;
    for(i = 0 ; i < count ; i++){
        pipe->data[nwrite++] = buffer[i];
    }
    pipe->nwrite = nwrite;
    return i;
}

int piperead(pipe_t pipe, char * buffer, int count){
    int i;
    for(i = 0 ; i < count ; i++){
        buffer[i] = pipe->data[pipe->nread++]; 
    }

    return i;
}