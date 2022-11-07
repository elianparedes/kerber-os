#include <phylo.h>
#include <ksemaphore.h>
#include <kstdio.h>
#include <kstdlib.h>
#include <kstring.h>

#define MAX_PHYLO 16
#define N 5
#define THINKING '.'
#define HUNGRY '.'
#define EATING 'E'

char state[MAX_PHYLO + 1];
sem_ptr state_sem;
sem_ptr mutex;

sem_ptr s[MAX_PHYLO];
char * args[MAX_PHYLO];
char name[32];

int pid[MAX_PHYLO];
int phylo_count;

void philosopher(int argc, char * argv[]);
void pick_forks(int i);
void put_forks(int i);
void test(int i);
void printState();
int left(int i);
int right(int i);
void add_phylo();
void remove_phylo();

int left(int i){
    return (i + phylo_count - 1) % phylo_count;
}

int right(int i){
    return (i + 1) % phylo_count;
}

void test(int i)
{
    if (state[i] == HUNGRY && state[left(i)] != EATING && state[right(i)] != EATING)
    {
        state[i] = EATING;
        printState();
        _sem_post(s[i]);
    }
}

void printState()
{
    _sem_wait(state_sem);
    printf("%s\n", state);
    _sem_post(state_sem);
}

void pick_forks(int i)
{
    _sem_wait(mutex);
    state[i] = HUNGRY;
    test(i);
    _sem_post(mutex);
    _sem_wait(s[i]);
}

void put_forks(int i)
{
    _sem_wait(mutex);
    state[i] = THINKING;
    test(left(i));
    test(right(i));
    _sem_post(mutex);
}

void aux_sleep(){
    for(int i = 0 ; i < 10000000 ; i++){
        for(int j = 0 ; j < 1 ; j++){

        }
    }
}

void philosopher(int argc, char * argv[]){
    int i = (int)strtol(argv[0],NULL,10);
    while(1){
        aux_sleep();
        pick_forks(i);
        aux_sleep();
        put_forks(i);
    }
}

void remove_phylo(){

    _sem_wait(mutex);

    if(phylo_count == 1){
        _sem_post(mutex);
        return;
    }

    int phylo = phylo_count-1;

    _kill(pid[phylo]);
    _sem_close(s[phylo]);

    phylo_count--;

    
    if(state[phylo] == EATING){
        test(phylo-1);
        test(0);
    }
    state[phylo] = '\0';

    _sem_post(mutex);
}

void receptionist(){
    int c;
    while((c = getchar()) != EOF){

        printf("%c\n",c);
        
        switch (c)
        {
        case 'A':
        case 'a':
            add_phylo();
            break;
        case 'R':
        case 'r':
            remove_phylo();
            break;

        default:
            break;
        }
    }
}

void add_phylo(){

    _sem_wait(mutex);

    if(phylo_count == MAX_PHYLO){
        _sem_post(mutex);
        return;
    }

    char aux[8];
    itoa(phylo_count,aux,10);
    strcpy(name+10,aux);
    s[phylo_count] = _sem_open(name,0);
    char * argv[1];
    argv[0] = aux;
    pid[phylo_count++] = _run(philosopher,1,argv);

    _sem_post(mutex);
}

void phylo(){

    mutex =_sem_open("mutex",1);
    state_sem =_sem_open("state_sem",1);

    strcpy(name,"phylo_sem ");

    phylo_count = 0;

    for(int i = 0 ; i < N ; i++){
        char aux[8];
        itoa(i,aux,10);
        strcpy(name+10,aux);
        s[i] = _sem_open(name,0);
        char * argv[1];
        argv[0] = aux;
        pid[phylo_count++] = _run(philosopher,1,argv);
    }
    
     _run(receptionist,0,NULL);

    _wait();

    for(int i = 0 ; i < N ; i++){
        _wait();
    }
}