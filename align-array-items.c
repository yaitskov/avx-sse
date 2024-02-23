/*  stream parser of decimal numbers.
 */
#include <stdio.h>
#include <stdlib.h>

static const int32_t a16[5] __attribute__ ((aligned (16))) = {0, 1, 2, 3, 4 };
static const int32_t a32[5] __attribute__ ((aligned (1024))) = {0, 1, 2, 3, 4 };



int main() {
  printf("start of a16 = %p;  first element %p; second element = %p\n", a16, &a16[0], &a16[1]);
  printf("start of a32 = %p;  first element %p; second element = %p\n", a32, &a32[0], &a32[1]);
  return 0;
}
