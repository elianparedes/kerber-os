#include <ipc_programs.h>
#include <kstdio.h>

#define TO_UPPER(X) ((X) - ('a' - 'A'))
#define IS_VOWEL(X)                                                            \
    (TO_UPPER(X) == 'A' || TO_UPPER(X) == 'E' || TO_UPPER(X) == 'I' ||         \
     TO_UPPER(X) == 'O' || TO_UPPER(X) == 'U')
#define SIZE_BUFFER 2048

void filter() {
    int16_t c;
    int i = 0;
    char buffer[SIZE_BUFFER];
    while (((c = getchar()) != (int16_t)-1) && i < (SIZE_BUFFER - 2)) {
        if (!IS_VOWEL(c)) {
            buffer[i++] = c;
            if (c == '\n') {
                buffer[i] = '\0';
                printf("%s", buffer);
                i = 0;
            }
        }
    }
    buffer[i++] = '\n';
    buffer[i] = '\0';
    printf("%s", buffer);
}

void cat() {
    int16_t c;
    int i = 0;
    char buffer[SIZE_BUFFER];
    while (((c = getchar()) != (int16_t)-1) && i < (SIZE_BUFFER - 2)) {
        buffer[i++] = c;
        if (c == '\n') {
            buffer[i] = '\0';
            printf("%s", buffer);
            i = 0;
        }
    }
    buffer[i++] = '\n';
    buffer[i] = '\0';
    printf("%s", buffer);
}

void wc() {
    int16_t c;
    int count = 1;
    int i = 0;
    char buffer[SIZE_BUFFER];
    while (((c = getchar()) != (int16_t)-1)) {
        buffer[i++] = c;
        if (c == '\n') {
            count++;
            buffer[i] = '\0';
            printf("%s", buffer);
            i = 0;
        }
    }

    buffer[i++] = '\n';
    buffer[i] = '\0';
    printf("%s", buffer);

    printf("TOTAL LINES: %d\n", count);
}