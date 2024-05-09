#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <malloc.h>

#define COUNT 0x10

void *func(int sz) {
    void* p[COUNT] = {};
    void* dummy;

    // allocate
    for (int i=0; i<COUNT; i++) {
        p[i] = malloc(sz);
        printf("malloc %#x: %p\n", sz, p[i]);
        memset(p[i], 'A' + i, sz);
    }

    // free
    for (int i=0; i<COUNT; i+=2) {
        memset(p[i], '\0', sz);
        printf("free   %#x: %p\n", sz, p[i]);
        free(p[i]);
    }

    printf("malloc %#x: %p\n", sz, malloc(sz));
}

int main(void) {
    func(0x80);
    while(1) {};
    return 0;
}

