#include <stdio.h>
__thread unsigned int x = 0xdeadbeef;

int main(void) {
    printf("%#x\n", x);
    return 0;
}
