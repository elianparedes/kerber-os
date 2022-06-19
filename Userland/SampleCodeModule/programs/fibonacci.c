#include <kstdio.h>
#include <kstdlib.h>

/*
 * Prints fibonacci sequence
 */

void fibonacci(char *arg) {
    int max = 200;

    int n1 = 0;
    int n2 = 1;

    if (arg != NULL && arg[0] != '\0')
        max = strtol(arg, NULL, 10);

    // first two terms of the sequence
    printf("fibonacci [%d]:    %d\n", 0, n1);
    printf("fibonacci [%d]:    %d\n", 1, n2);

    int next = n1 + n2;
    int i = 2;
    while (i < max) {
        if (next < 0) {
            n1 = 0;
            n2 = 1;
            next = n1 + n2;
            i = 0;
        }
        printf("fibonacci [%d]      %d\n", i, next);
        n1 = n2;
        n2 = next;
        next = n1 + n2;
        i++;
    }
}