#include <cat.h>
#include <kstdio.h>

#define SIZE_BUFFER 2048

void cat(){ 
    int16_t c;
    int i = 0;
    char buffer[SIZE_BUFFER];
    while(((c = getchar()) != (int16_t)-1) &&  i < (SIZE_BUFFER - 2)){
        buffer[i++] = c;
        if(c == '\n'){
            buffer[i] = '\0';
            printf("%s",buffer);
            i = 0;
        }
    }
    buffer[i++] = '\n';
    buffer[i] = '\0';
    printf("%s",buffer);
}