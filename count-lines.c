#include <stdio.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include <errno.h>
#include <fcntl.h>
#include <sys/mman.h>
#include <stdlib.h>
#include <time.h>
#include <string.h>
#include <immintrin.h>

  //This seems to be our dictionary
static const unsigned char input[16] __attribute__((aligned(32))) = "123456\n89,012345";

//This seems to be the one that gives the index
static const unsigned char stopChars[16] __attribute__((aligned(32))) = ",\n";

#define simd_compare_string_mode (_SIDD_UBYTE_OPS | _SIDD_CMP_EQUAL_ANY) // | _SIDD_NEGATIVE_POLARITY | _SIDD_BIT_MASK)

int main() {
  const __m128i simd_a = _mm_loadu_si128((__m128i *)input);
  const __m128i simd_b = _mm_loadu_si128((__m128i *)stopChars);

  int result = _mm_cmpistri(simd_b, simd_a, simd_compare_string_mode);

  printf("Skip %d bytes\n", result);

  return 0;
}
