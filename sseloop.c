#include <stdio.h>
#include <immintrin.h>
#include "immediate.h"

__m128i srli_si128(__m128i x, int n) {
  immediate(return _mm_srli_si128, x, n);
}

inline __m128i testUnrollSse(__m128i x, int n) {
#pragma GCC unroll 41
  for (int i = 0; i < n; ++i) {
    x = _mm_srli_si128(x, i);
  }
  return x;
}

void print_mm_epi32(__m128i r) {
  printf("%d %d %d %d\n",
         _mm_extract_epi32(r, 0),
         _mm_extract_epi32(r, 1),
         _mm_extract_epi32(r, 2),
         _mm_extract_epi32(r, 3));
}
__m128i x;

int main() {
  int a;
  scanf("%d", &a);
  printf("a = %d\n", a);
  x = _mm_set_epi32(a,a,a,a);
  print_mm_epi32(x);
  __m128i r = srli_si128(x, a);
  immediate(r = _mm_srli_si128, r, a);
  print_mm_epi32(r);
  return 0;
}
