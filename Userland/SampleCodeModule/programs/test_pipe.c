// This is a personal academic project. Dear PVS-Studio, please check it.
// PVS-Studio Static Code Analyzer for C, C++ and C#: http://www.viva64.com
#include <infopipe.h>
#include <kpipe.h>
#include <ksemaphore.h>
#include <kstdio.h>
#include <test_pipe.h>

#include <ipc_programs.h>

sem_ptr sem_shell;

void process_left() {
    _sem_wait(sem_shell);
    int fd[2];
    if (_create_pipe("my_pipe", fd) == -1)
        _open_pipe("my_pipe", fd);
    _sem_post(sem_shell);

    _dup2(fd[1], 1);
    _close(fd[1]);
    _close(fd[0]);

    printf("Hola mundo\nescribo otra\n");

    _exit(0);
}

void process_right() {
    int fd[2];
    _sem_wait(sem_shell);
    if (_open_pipe("my_pipe", fd) == -1)
        _create_pipe("my_pipe", fd);
    _sem_post(sem_shell);

    _dup2(fd[0], 0);
    _close(fd[0]);
    _close(fd[1]);

    // cat();
    // filter();
    wc(0,NULL);

    _exit(0);
}

int test_pipes(int argc, char *argv[]) {
    sem_shell = _sem_open("shell_sem", 1);
    _run(process_left, 0, NULL);
    _run(process_right, 0, NULL);
    _wait();
    _wait();
    _sem_close(sem_shell);

    return 0;
}