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
        printf("%#x: %p\n", sz, p[i]);
        memset(p[i], 'A' + i, sz);
    }

    // free
    for (int i=0; i<COUNT; i+=2) {
        memset(p[i], '\0', sz);
        free(p[i]);
    }

}

int main(void) {
    func(0x10);
    while(1) {};
    return 0;
}

