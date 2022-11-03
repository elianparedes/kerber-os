#include <cat.h>
#include <kstdio.h>

#define SIZE_BUFFER 2048

void cat(){ 
    int c;
    int i = 0;
    char buffer[SIZE_BUFFER];
    while((c = getchar()) != -1 &&  i < SIZE_BUFFER){
        buffer[i++] = c;
        if(c == '\n'){
            buffer[i] = '\0';
            printf("%s",buffer);
            i = 0;
        }
    }
}