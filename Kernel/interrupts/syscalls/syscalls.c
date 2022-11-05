// This is a personal academic project. Dear PVS-Studio, please check it.
// PVS-Studio Static Code Analyzer for C, C++ and C#: http://www.viva64.com
#include <interrupts.h>
#include <keyboard.h>
#include <pmm.h>
#include <registers.h>
#include <scheduler.h>
#include <semaphore/semaphore.h>
#include <syscalls.h>
#include <video.h>
#include <semaphore/semaphore.h>
#include <dataDescriptor.h>
#include <pmm.h>
#include <pipe/pipe.h>
#include <lib/defs.h>

#define ADDRESS_LIMIT 0xFFFFFFFF

enum DISTRIBUTION
{
    FULL_DISTRIBUTION = 0,
    SPLIT_DISTRIBUTION
};

int16_t read(int fd, char *buffer, uint16_t count)
{
    if (fd < 0)
        return -2;

    process_t *current_process = get_current_process();

    if(fd >= current_process->dataD_index )
        return -2;

    dataDescriptor_t dataD = current_process->dataDescriptors[fd];
    if(getMode_dataDescriptor(dataD) != READ_MODE)
        return -2;

    pipe_t pipe;
    uint16_t i;
    char aux[BUFFER_SIZE];

    switch (getDataType_dataDescriptor(dataD))
    {
    case PIPE_T:
         pipe = getPipe_dataDescriptor(dataD);
        return piperead(pipe, buffer, count);

    default:
        kbd_clear_buffer();
        _sti();
        while (count > kbd_get_current_index())
        {
        }
        _cli();
        
        int code = kbd_get_buffer(aux);
        if(code == EOF)
            return EOF;
        
        for (i = 0; i < count; i++)
        {
            buffer[i] = aux[i];
        }
        return i - 1;
    }
}

int16_t write(int fd, char *buffer, uint16_t count)
{

    if (fd < 0)
        return -2;

    process_t *current_process = get_current_process();

    if(fd >= current_process->dataD_index )
        return -2;

    dataDescriptor_t dataD = current_process->dataDescriptors[fd];
    if(getMode_dataDescriptor(dataD) != WRITE_MODE)
        return -1;

    pipe_t pipe;
    context_id_t gc;
    uint16_t i;
    char c;

    switch (getDataType_dataDescriptor(dataD))
    {
    case PIPE_T:
        pipe = getPipe_dataDescriptor(dataD);
        return pipewrite(pipe, buffer, count);

    default:
         gc = current_process->g_context;

         i = 0;
        while (i < count)
        {
            c = buffer[i];
            if (c == '\n')
            {
                gprint_new_line(gc);
            }
            else
            {
                gprint_char(c, gc);
            }
            i++;
        }
        return i;
        break;
    }
}

uint8_t sys_gettime(time_t *struct_time, int utc_offset)
{
    // set_UTC_offset(utc_offset);
    get_struct_time();
    return SUCCESS;
}

void sys_exit(int error_code)
{
    exit_process();
}

void sys_switch_screen_mode(int mode)
{
    if (mode == FULL_DISTRIBUTION)
    {
        full_screen_distribution();
    }
    if (mode == SPLIT_DISTRIBUTION)
    {
        split_screen_distribution();
    }
}

void sys_clear_screen()
{
    clear_screen();
}

int sys_run(void *main, int argc, char *argv[]) {
    return add_process(main, argc, argv);
}

void sys_delete_char()
{
    process_t *current_process = get_current_process();
    context_id_t gc = current_process->g_context;

    gdelete_char(gc);
}

int sys_cntrl_pressed()
{
    return kbd_is_cntrl_pressed();
}

int sys_copy_cpu_state(cpu_state_t *cpu_ptr, request_t request)
{
    return copy_cpu_state(cpu_ptr, request);
}

uint8_t sys_cntrl_listener(uint8_t *listener)
{
    kbd_sets_cntrl_listener(listener);
    return SUCCESS;
}

void sys_kill(int pid)
{
    kill_process(pid);
}

void sys_wait(int child, int *pstatus)
{
    if (child > 1)
    {
        *pstatus = -1;
        return;
    }

    process_t *process = get_current_process();
    if (child == 0)
        *pstatus = process->l_child != NULL;
    else if (child == 1)
        *pstatus = process->r_child != NULL;
}

void sys_pause(int pid)
{
    process_t *process = get_process(pid);
    if (process != NULL)
        process->status = !process->status;
    return;
}

uint16_t sys_get_mem(uint8_t *address, uint8_t *buffer, uint16_t count)
{
    int i;
    for (i = 0; i < count; i++)
    {
        if ((uint64_t)address > ADDRESS_LIMIT)
        {
            return i;
        }
        buffer[i] = (*address);
        address++;
    }
    return i;
}

void sys_focus(int pid)
{
    process_t *process = get_process(pid);
    if (process != NULL)
    {
        gfocus(process->g_context);
    }
}

void sys_sched_yield() {
    _force_schedule();
}

void sys_wait2() {
    return wait_process();
}

sem_ptr sys_sem_open(char *name, int value) {
    return sem_open(name, value);
}

int sys_sem_wait(sem_ptr sem) {
    return sem_wait(sem);
}

int sys_sem_post(sem_ptr sem) {
    return sem_post(sem);
}

void sys_close(unsigned int fd){
    process_t * process = get_current_process();
    if(fd >= process->dataD_index)
        return;
    close_dataDescriptor(process->dataDescriptors[fd]);
    process->dataDescriptors[fd] = NULL;
}

int sys_create_pipe(char * name, int fd[2]){
    return create_pipe(name,fd);
}

int sys_open_pipe(char * name, int fd[2]){
    return open_pipe(name,fd);
}

int sys_info_pipe(char * name, pipe_info_t * info){
    return info_pipe(name,info);
}

int sys_info_all_pipes(pipe_info_t * info[] , unsigned int size){
    return info_all_pipes(info,size);
}

int sys_dup2(unsigned int oldfd , unsigned int newfd){
    return dup2(oldfd,newfd);
}

int sys_sem_close(sem_ptr sem){
    return sem_close(sem);
}

int sys_get_semaphores(copy_sem_t *sems[]) {
    return get_semaphores(sems);
}

void *sys_malloc(size_t size) {
    return kmalloc(size);
}

void sys_free(void *ptr) {
    kfree(ptr);
}
