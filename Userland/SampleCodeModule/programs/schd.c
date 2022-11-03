#include <kstdio.h>
#include <schd.h>

void dummy() {
    while (1)
        ;
}

void schd() {
    _run(dummy, 0, NULL);
    _run(dummy, 0, NULL);
    _wait2();
    return 0;
}