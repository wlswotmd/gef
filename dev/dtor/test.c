#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
typedef void (*dtor_func) (void *);
extern void *__dso_handle __attribute__ ((__visibility__ ("hidden")));
extern int __cxa_thread_atexit_impl (dtor_func, void *, void *);

typedef struct {
    void *val;
} A;

void A_dtor(void *obj) {
}

int main(int argc, char* argv[]) {
    // tls_dtor_list
    static __thread A b;
    //__cxa_thread_atexit_impl((dtor_func)A_dtor, &b, __dso_handle);
    __cxa_thread_atexit_impl((dtor_func)0x41414141, &b, __dso_handle);

    // __exit_funcs
    atexit((void (*)(void))0x42424242);
    on_exit((void (*)(int, void*))0x43434343, NULL);

    // __quick_exit_funcs
    at_quick_exit((void (*)(void))0x44444444);

    sleep(3);
    return 0;
}

