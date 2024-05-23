#include <stdio.h>
_Thread_local unsigned int x = 0xdeadbeef;
// __thread unsigned int x = 0xdeadbeef;

int main(void) {
    printf("%#x\n", x);
    return 0;
}
