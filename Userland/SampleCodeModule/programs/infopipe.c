// This is a personal academic project. Dear PVS-Studio, please check it.
// PVS-Studio Static Code Analyzer for C, C++ and C#: http://www.viva64.com
#include <infopipe.h>
#include <kpipe.h>
#include <kmman.h>
#include <kstdio.h>
#include <kstring.h>
#include <kstdlib.h>

#define MAX_PIPES 32

int columns[7];

static void print_tabs(int amount){
    char buffer[80] = {0};
    int i = 0;
    while(i < amount){
        buffer[i++] = ' '; 
    }
    printf("%s",buffer);
}

static void print_header(){
        printf("NAME           | BLOCKED | BLOCKED PIDS      | R_OPEN  | W_OPEN  | R   | W   \n");
}

static int get_spaces(char * str , int col){
    return columns[col] - strlen(str);
}

static void pids_to_str(int pids[] , char dest[],int size){

    int i = 0;
    for (int j = 0 ; i < size ; j++){
        char aux[8] = {0};
        itoa(pids[j],aux,10);
        int k = 0;
        while(aux[k] != 0){
            dest[i++] = aux[k++];
        }
        dest[i++] = ' ';
    }
    dest[i++] = '\0';
}

static void print_line(pipe_info_t * info){

    char aux[80] = {0};

    // NAME
    int len = 0;
    if((len = strlen(info->name)) > columns[0]){
        strcpy(aux,info->name);
        aux[columns[0]-1]='.';
        aux[columns[0]-2]='.';
        aux[columns[0]-3]='.';
        aux[columns[0]] = '\0';
        printf("%s ",aux);
    }
    else{
        printf("%s ",info->name);
    }
    print_tabs(get_spaces(info->name,0));
    printf("| ");

    //BLOCKED
    itoa(info->blocked_count,aux,10);
    printf("%s",aux);
    print_tabs(get_spaces(aux,1));
    printf("| ");

    //BLOCKED_PIDS
    pids_to_str(info->blocked_pid,aux,info->blocked_count);
    if(info->blocked_count == 0){
        print_tabs(get_spaces("",2));
    } 
    else{
        printf("%s",aux);
        print_tabs(get_spaces(aux,2));
    }
    printf("| ");

    //R_OPEN
    itoa(info->readopen,aux,10);
    printf("%s",aux);
    print_tabs(get_spaces(aux,3));
    printf("| ");

    //W_OPEN
    itoa(info->writeopen,aux,10);
    printf("%s",aux);
    print_tabs(get_spaces(aux,4));
    printf("| ");

    //R
    itoa(info->nread,aux,10);
    printf("%s",aux);
    print_tabs(get_spaces(aux,5));
    printf("| ");

    //W
    itoa(info->nwrite,aux,10);
    printf("%s",aux);
    print_tabs(get_spaces(aux,6));
    putchar('\n');

}

static void init_columns_size(){
    columns[0] = strlen("NAME          ");
    columns[1] = strlen(" BLOCKED");
    columns[2] = strlen(" BLOCKED PIDS     ");
    columns[3] = strlen(" R_OPEN ");
    columns[4] = strlen(" W_OPEN ");
    columns[5] = strlen(" R  ");
    columns[6] = strlen(" W  ");
}

void info_pipe(char * name){
    
    pipe_info_t * pipe_info = (pipe_info_t *)_malloc(sizeof(pipe_info_t));
    _info_pipe(name,pipe_info);
    
    init_columns_size();
    print_header();
    print_line(pipe_info);

    putchar('\n');
}


int info_all_pipes(int argc, char *argv[]){

    pipe_info_t * arr[MAX_PIPES] = {0};
    for(int i = 0 ; i < MAX_PIPES ; i++){
        arr[i] = (pipe_info_t *)_malloc(sizeof(pipe_info_t));
    }
    int amount = _info_all_pipes(arr,MAX_PIPES);

    init_columns_size();
    print_header();

    for(int i = 0 ; i < amount ; i++){
        print_line(arr[i]);
    }

    putchar('\n');

    for(int i = 0 ; i < MAX_PIPES ; i++){
        _free(arr[i]);
    }

    return 0;
}