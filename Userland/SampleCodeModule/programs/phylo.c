#include <phylo.h>
#include <ksemaphore.h>
#include <kstdio.h>
#include <kstdlib.h>
#include <kstring.h>

#define MAX_PHYLO 10
#define N 5
#define PENSANDO '.'
#define HAMBRIENTO '.'
#define COMIENDO 'E'

char estado[MAX_PHYLO + 1];
sem_ptr state;
sem_ptr mutex;
sem_ptr s[MAX_PHYLO];
char * args[MAX_PHYLO];
char name[32];

int pid[MAX_PHYLO];
int phylo_count;

void filosofo(int argc, char * argv[]);
void tomar_tenedores(int i);
void poner_tenedores(int i);
void probar(int i);
void printState();
int izquierdo(int i);
int derecho(int i);
void add_phylo();
void remove_phylo();

int izquierdo(int i){
    return (i + phylo_count - 1) % phylo_count;
}

int derecho(int i){
    return (i + 1) % phylo_count;
}

void probar(int i)
{
    if (estado[i] == HAMBRIENTO && estado[izquierdo(i)] != COMIENDO && estado[derecho(i)] != COMIENDO)
    {
        estado[i] = COMIENDO;
        printState();
        _sem_post(s[i]);
    }
}

void printState()
{
    _sem_wait(state);
    printf("%s\n", estado);
    _sem_post(state);
}

void tomar_tenedores(int i)
{
    _sem_wait(mutex);
    estado[i] = HAMBRIENTO;
    probar(i);
    _sem_post(mutex);
    _sem_wait(s[i]);
}

void poner_tenedores(int i)
{
    _sem_wait(mutex);
    estado[i] = PENSANDO;
    probar(izquierdo(i));
    probar(derecho(i));
    _sem_post(mutex);
}



void filosofo(int argc, char * argv[]){
    int i = (int)strtol(argv[0],NULL,10);
    while(1){
        _sleep_time(1);
        tomar_tenedores(i);
        _sleep_time(1);
        poner_tenedores(i);
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

    
    if(estado[phylo] == COMIENDO){
        probar(phylo-1);
        probar(0);
    }
    estado[phylo] = '\0';
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
    s[phylo_count] = _sem_open(name,1);
    char * argv[1];
    argv[0] = aux;
    pid[phylo_count++] = _run(filosofo,1,argv);

    _sem_post(mutex);
}

void phylo(){

    mutex =_sem_open("mutex",1);
    state =_sem_open("state",1);

    strcpy(name,"phylo_sem ");

    phylo_count = 0;

    for(int i = 0 ; i < N ; i++){
        char aux[8];
        itoa(i,aux,10);
        strcpy(name+10,aux);
        s[i] = _sem_open(name,1);
        char * argv[1];
        argv[0] = aux;
        pid[phylo_count++] = _run(filosofo,1,argv);
    }
    
     _run(receptionist,0,NULL);

    _wait();

    for(int i = 0 ; i < N ; i++){
        _wait();
    }
}