#include <kstdio.h>
#include <schd.h>

void dummy() {
    while (1)
        ;
}

void schd() {
    char *args[] = {"dummy"};
    _run(dummy, 1, args);
    _run(dummy, 1, args);
    _wait();
    _wait();
    return 0;
}