#include<kstdio.h>
#include<ksemaphore.h>
#include <kstring.h>

sem_ptr room;
sem_ptr chopstick[9];

void philosopher(int argc, char * argv[]);
void eat(int);
void receptionist();

int phylo_count;

void phylo()
{
	int i;

	room = _sem_open("phylo-room",4);
	
    char name[32];

	for(phylo_count=0;phylo_count<5;phylo_count++){
        strcpy(name,"chopstick ");
        name[10] = phylo_count + '0';
        name[11] = '\0';
		chopstick[i] = _sem_open(name,1);
    }

    char * argv0[1];
    char id0[1];
    id0[0] = '0';
    argv0[0] = id0;
    _run(philosopher,1,argv0);

    char * argv1[1];
    char id1[1];
    id1[0] = '1';
    argv1[0] = id1;
    _run(philosopher,1,argv1);

    char * argv2[1];
    char id2[1];
    id2[0] = '2';
    argv2[0] = id2;
    _run(philosopher,1,argv2);

    char * argv3[1];
    char id3[1];
    id3[0] = '3';
    argv3[0] = id3;
    _run(philosopher,1,argv3);

    char * argv4[1];
    char id4[1];
    id4[0] = '4';
    argv4[0] = id4;
    _run(philosopher,1,argv4);

	for(i=0;i<5;i++)
		_wait2();

    _run(receptionist,0,NULL);
}

/*
void receptionist(){
    int c;

    char name[32];
    strcpy(name,"chopstick ");
    name[10] = phylo_count + '0';
    name[11] = '\0';

    while((c = getchar()) != -1){
        if( c == 'a'){
            lock(phylo_count);

            if(phylo_count == 9){
                unlock(phylo_count);
                return;
            }

            chopstick[phylo_count++] = _sem_open(name,1);
            _sem_post(room);
            _run(philosopher,1,argv0);
            unlock(phylo_count);
        }
        else if (c == 'r'){

        }
    }
} */

void philosopher(int argc, char * argv[])
{
    while(1){
	int phil= argv[0][0] - '0';

	_sem_wait(room);
    _sleep_time(3);
	printf("\nPhilosopher %d has entered room",phil);
	_sem_wait(chopstick[phil]);
    lock(phylo_count);
	_sem_wait(chopstick[(phil+1)%phylo_count]);

    _sleep_time(3);
	eat(phil);

    _sleep_time(3);
	printf("\nPhilosopher %d has finished eating",phil);

	_sem_post(chopstick[(phil+1)%phylo_count]);
    unlock(phylo_count);
	_sem_post(chopstick[phil]);
	_sem_post(room);
    }
}

void eat(int phil)
{
	printf("\nPhilosopher %d is eating",phil);
}