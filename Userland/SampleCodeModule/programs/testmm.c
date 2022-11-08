// This is a personal academic project. Dear PVS-Studio, please check it.
// PVS-Studio Static Code Analyzer for C, C++ and C#: http://www.viva64.com
#include <kmman.h>
#include <kstdio.h>
#include <kstdlib.h>
#include <kstring.h>
#include <test_util.h>
#include <testmm.h>

#define MAX_BLOCKS 80

typedef struct MM_rq {
    void *address;
    uint32_t size;
} mm_rq;

int test_mm(int argc, char *argv[]) {
    mm_rq mm_rqs[MAX_BLOCKS];
    uint8_t rq;
    uint32_t total;
    uint64_t max_memory;

    if (argc != 1) {
        printf("Error argc\n");
        return -1;
    }

    if ((max_memory = satoi(argv[0])) <= 0) {
        printf("Error max_mem\n");
        return -1;
    }

    rq = 0;
    total = 0;

    // Request as many blocks as we can
    while (rq < MAX_BLOCKS && total < max_memory) {
        mm_rqs[rq].size = GetUniform(max_memory - total - 1) + 1;
        mm_rqs[rq].address = _malloc(mm_rqs[rq].size);

        printf("block [%d] size: %d, address: %d\n", rq, (int)mm_rqs[rq].size,
               mm_rqs[rq].address);

        if (mm_rqs[rq].address) {
            total += mm_rqs[rq].size;
            rq++;
        }
    }

    // Set
    uint32_t i;
    for (i = 0; i < rq; i++)
        if (mm_rqs[i].address)
            memset(mm_rqs[i].address, i, mm_rqs[i].size);

    // Check
    for (i = 0; i < rq; i++) {
        if (mm_rqs[i].address) {
            if (!memcheck(mm_rqs[i].address, i, mm_rqs[i].size)) {
                printf("test_mm ERROR\n");
                return -1;
            }
        }
    }

    // Free
    for (i = 0; i < rq; i++)
        if (mm_rqs[i].address)
            _free(mm_rqs[i].address);

    printf("testmm: memory test passed!\n");
    return 0;
}