#include <stdio.h>
#include "count.h" //includes info from count.h

int main(void) {
    printf("%d\n", count);
    increment();
    printf("%d\n", count);
    return 0;
}