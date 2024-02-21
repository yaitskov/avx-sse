#include <stdio.h>
#include <immintrin.h>

static const char input[32] __attribute__ ((aligned (32))) = "1234";

void m256_print_epi32(const char* p, __m256i* x) {
  printf("%s  %6d %6d %6d %6d %6d %6d %6d %6d\n",
         p,
         _mm256_extract_epi32(*x, 0),
         _mm256_extract_epi32(*x, 1),
         _mm256_extract_epi32(*x, 2),
         _mm256_extract_epi32(*x, 3),
         _mm256_extract_epi32(*x, 4),
         _mm256_extract_epi32(*x, 5),
         _mm256_extract_epi32(*x, 6),
         _mm256_extract_epi32(*x, 7)
         );
}

int main() {
  // __m256i result = _mm256_set_epi64x(0, 0, 0, 0);
  //  __m256i digitKs = _mm256_set_epi32(10_000_000, 1000_000, 100_000, 10_000, 1000, 100, 10, 1);
  __m256i digitBase10 = _mm256_set_epi32(0, 0, 0, 0, '0', '0', '0', '0');
  __m256i digitKs = _mm256_set_epi32(0, 1, 0, 100, 0, 10, 0, 1000);
  // __m256i i = _mm256_load_si256((__m256i*) input);
  __m128i i = _mm_load_si128((__m128i*) input);
  __m256i cvt8_32 = _mm256_cvtepi8_epi32(i);
  m256_print_epi32("cvt8-32", &cvt8_32);
  // i = _m256_srli_epi64(i, 0);
  // _m256_extract_epi64(i,
  m256_print_epi32("base-10", &digitBase10);
  __m256i sub = _mm256_sub_epi32(cvt8_32, digitBase10);
  m256_print_epi32("sub    ", &sub);
  // shuffle_epi32 cannot move data between 128 buckets
  // let's try _mm256_extracti128_si256 and _mm256_inserti128_si256
  __m256i sub2 = _mm256_inserti128_si256(
                                         sub,
                                         _mm_srli_si128(
                                                        _mm256_extracti128_si256(sub, 0),
                                                        4
                                                        ),
                                         1);
  m256_print_epi32("sub2   ", &sub2);
                          // _mm256_shuffle_epi32(sub, 0b_03_02_01_00_03_02_01_00);
  m256_print_epi32("digitKs", &digitKs);
  __m256i p = _mm256_mul_epi32(sub2, digitKs);

  long r0 = _mm256_extract_epi64(p, 0);
  long r1 = _mm256_extract_epi64(p, 1);
  long r2 = _mm256_extract_epi64(p, 2);
  long r3 = _mm256_extract_epi64(p, 3);
  long result = r0 + r1 + r2 + r3;
  printf("%ld; %s = %ld = %ld + %ld + %ld + %ld\n", sizeof(long), input, result, r0, r1, r2, r3);
  return 0;
}
