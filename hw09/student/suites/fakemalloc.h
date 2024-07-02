// Need stdlib.h #included before #defines, otherwise the preprocessor expands
// the malloc and calloc prototypes when they're #included later.
#include <stdlib.h>

#define MALLOC_COUNT_CHECK (mallocs_until_fail == -1 || (mallocs_until_fail && mallocs_until_fail--))

#define malloc(n)     (MALLOC_COUNT_CHECK ? malloc2110(n) : NULL)
#define calloc(n, s)  (MALLOC_COUNT_CHECK ? calloc(n, s) : NULL)
#define realloc(n, s) (MALLOC_COUNT_CHECK ? realloc(n, s) : NULL)

extern int mallocs_until_fail;
void *malloc2110(size_t);
