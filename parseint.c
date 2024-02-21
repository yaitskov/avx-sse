#include <stdio.h>
#include <immintrin.h>

static const char input[32] __attribute__ ((aligned (32))) = "1234";

int main() {
  // __m256i result = _mm256_set_epi64x(0, 0, 0, 0);
  //  __m256i digitKs = _mm256_set_epi32(10_000_000, 1000_000, 100_000, 10_000, 1000, 100, 10, 1);
  __m256i digitBase10 = _mm256_set_epi32(0, 0, 0, 0, '0', '0', '0', '0');
  __m256i digitKs = _mm256_set_epi32(1, 1, 1, 0, 1, 10, 1, 1000);
  // __m256i i = _mm256_load_si256((__m256i*) input);
  __m128i i = _mm_load_si128((__m128i*) input);

  // i = _m256_srli_epi64(i, 0);
  // _m256_extract_epi64(i,
  __m256i cvt = _mm256_sub_epi32(_mm256_cvtepi8_epi32(i), digitBase10);
  printf("cvt %d %d %d %d\n", _mm256_extract_epi32(cvt, 0), _mm256_extract_epi32(cvt, 1), _mm256_extract_epi32(cvt, 2), _mm256_extract_epi32(cvt, 3));
  __m256i p = _mm256_mul_epi32(cvt, digitKs);

  long r0 = _mm256_extract_epi64(p, 0);
  long r1 = _mm256_extract_epi64(p, 1);
  long r2 = _mm256_extract_epi64(p, 2);
  long r3 = _mm256_extract_epi64(p, 3);
  long result = r0 + r1 + r2 + r3;
  printf("%d; %s = %d = %d + %d + %d + %d\n", sizeof(long), input, (int) result, r0, r1, r2, r3);
  return 0;
}
