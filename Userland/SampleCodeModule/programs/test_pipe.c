#include <test_pipe.h>
#include <kstdio.h>
#include <infopipe.h>
#include <kpipe.h>
#include <ksemaphore.h>
#include <cat.h>

sem_ptr sem_shell;

void process_left(){
    _sem_wait(sem_shell);
    int fd[2];
    _create_pipe("my_pipe",fd);
    _sem_post(sem_shell);

    _dup2(fd[1],1);
    _close(fd[1]);
    _close(fd[0]);

    //printf("Hola mundo soy el izq y continuo escribiendo para el cataaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaa\n");

    _exit(0);

}

void process_right(){
    int fd[2];
    _sem_wait(sem_shell);
    _open_pipe("my_pipe",fd);
    _sem_post(sem_shell);

    _dup2(fd[0],0);
    _close(fd[0]);
    _close(fd[1]);

    cat();

/*
    char buffer[256];
    printf("Ingrese un texto:\n");
    int num = _read(0,buffer,256);
    for(int i = 0 ; i < num ; i++){
        if(buffer[i] == 'a' || buffer[i] == 'e' || buffer[i] == 'i' || buffer[i] == 'o' || buffer[i] == 'u' || 
        buffer[i] == 'A' || buffer[i] == 'E' || buffer[i] == 'I' || buffer[i] == 'O' || buffer[i] == 'U')
            buffer[i] = '-';
    }

    printf("%s\n",buffer);
    */    
    _exit(0);

}

void test_pipes(){
    sem_shell = _sem_open("shell_sem",1);
    _run(process_left,NULL);
    _run(process_right,NULL);
    _wait2();
    _wait2();
    _sem_close(sem_shell);
}