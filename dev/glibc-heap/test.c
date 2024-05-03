// gcc test.c -lpthread
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <malloc.h>
#include <pthread.h>

#define COUNT 0x30

void *func(void* arg) {
    void* p[COUNT*2] = {};
    void* dummy;

    // allocate
    for (int i=0; i<COUNT; i++) {
        p[i*2] = malloc(0x10); // tcache, fastbin, small bin
        dummy = malloc(0x20);
        p[i*2+1] = malloc(0x1000); // unsorted, large bin
        dummy = malloc(0x20);
    }

    // free
    for (int i=0; i<COUNT / 3 * 2; i+=2) {
        free(p[i*2]); // tcache, fastbin
        if (i < COUNT / 3)
            free(p[i*2+1]); // unsorted
    }

    // consolidate (fastbin -> small bin; unsorted -> large bin)
    void *q = malloc(0x10000);

    // free
    for (int i=COUNT / 3 * 2; i<COUNT; i+=2) {
        free(p[i*2]); // fastbin
        free(p[i*2+1]); // unsorted
    }

    while(1) {};
}

int main(void) {
    pthread_t th[2];
    for (int i=0; i<2; i++) {
        pthread_create(&th[i], NULL, func, NULL);
    }
    func(NULL);

    return 0;
}

