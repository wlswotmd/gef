// gcc test2.c -lpthread
#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>

void cleanup_func(void *arg) {
    printf("called: %s \n",(char *)arg);
}

void* func(void *arg) {
    // It's actually a macro, which is stored on the stack, making it difficult to dump.
    pthread_cleanup_push(cleanup_func, "cleanup_func (1)");
    pthread_cleanup_push(cleanup_func, "cleanup_func (2)");
    pthread_exit((void*)2);
    // never reached
    pthread_cleanup_pop(0);
    pthread_cleanup_pop(0);
}

int main(int argc, char* argv[]) {
    pthread_t tid;
    void* tret;
    pthread_create(&tid, NULL, func, (void*) 1);
    pthread_join(tid, &tret);
    return 0;
}
