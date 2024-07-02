#include <stdlib.h>


/** How to run Valgrind in command line:
 *  1. Open Docker
 *  2. Type gcc -g -o0 -o <output-file> Lab21Demo.c 
 *  3. Type valgrind --leak-check=yes ./<output-file>
 */ 
int f(void) {
     int* x = (int*) malloc(10 * sizeof(int));
     x[0] = 5;
     x[10] = 0;       // problem 1: heap block overrun
     return x[0];    // problem 2: memory leak -- x not freed
}                  

int main(void) {
     printf("%d\n", f());
     return 0;
}





