#include <kstdio.h>

/*
* Prints fibonacci sequence
*/

void fibonacci(){
    int n1=0;
    int n2=1;

    printf("%d %d ", n1, n2);  //first two terms of the sequence

    int next= n1 + n2;

    while (1){
        if (next <0){
            n1=0;
            n2=1;
            next=n1+n2;
        }
        printf("%d ", next);
        n1=n2;
        n2=next;
        next= n1 + n2;
    }
}