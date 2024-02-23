/*

__m256i _mm256_mask_i32gather_epi32  (__m256i src, int const* base_addr, __m256i vindex, __m256i mask, const int scale)
__m256i _mm256_mmask_i32gather_epi32 (__m256i src, __mmask8 k, __m256i vindex, void const* base_addr,  const int scale)
 */
#include <sys/time.h>
#include <stdio.h>
#include <stdlib.h>
#include "avxsse.h"


static const int32_t posScaledDigits[8 * 10] __attribute__ ((aligned (32))) = {
  /* 0 */ 0, 1, 2, 3, 4, 5, 6, 7, 8, 9,
  /* 1 */ 0, 10, 20, 30, 40, 50, 60, 70, 80, 90,
  /* 2 */ 0, 100, 200, 300, 400, 500, 600, 700, 800, 900,
  /* 3 */ 0, 1000, 2000, 3000, 4000, 5000, 6000, 7000, 8000, 9000,
  /* 4 */ 0, 10000, 20000, 30000, 40000, 50000, 60000, 70000, 80000, 90000,
  /* 5 */ 0, 100000, 200000, 300000, 400000, 500000, 600000, 700000, 800000, 900000,
  /* 6 */ 0, 1000000, 2000000, 3000000, 4000000, 5000000, 6000000, 7000000, 8000000, 9000000,
  /* 7 */ 0,  10000000, 20000000, 30000000, 40000000, 50000000, 60000000, 70000000, 80000000,                                                                 90000000
};

static const int64_t posScaledDigitsLong[8 * 10] __attribute__ ((aligned (32))) = {
  /* 8 */ 0,  100000000, 200000000, 300000000, 400000000, 500000000, 600000000, 700000000,        800000000,                                                         900000000,
  /* 9 */ 0,  1000000000, 2000000000, 3000000000, 4000000000, 5000000000, 6000000000, 7000000000,        8000000000,                                                 9000000000,
  /* 10 */ 0, 10000000000, 20000000000, 30000000000, 40000000000, 50000000000, 60000000000, 70000000000, 80000000000,                                         90000000000,
  /* 11 */ 0, 100000000000, 200000000000, 300000000000, 400000000000, 500000000000, 600000000000, 700000000000, 800000000000,                                 900000000000,
  /* 12 */ 0, 1000000000000, 2000000000000, 3000000000000, 4000000000000, 5000000000000, 6000000000000, 7000000000000, 8000000000000,                         9000000000000,
  /* 13 */ 0, 10000000000000, 20000000000000, 30000000000000, 40000000000000, 50000000000000, 60000000000000, 70000000000000, 80000000000000,                 90000000000000,
  /* 14 */ 0, 100000000000000, 200000000000000, 300000000000000, 400000000000000, 500000000000000, 600000000000000, 700000000000000, 800000000000000,         900000000000000,
  /* 15 */ 0, 1000000000000000, 2000000000000000, 3000000000000000, 4000000000000000, 5000000000000000, 6000000000000000, 7000000000000000, 8000000000000000, 9000000000000000
};

/*   /\* 16 *\/ 0, 1000000000000000, 2000000000000000, 3000000000000000, 4000000000000000, 5000000000000000, 6000000000000000, 7000000000000000, 8000000000000000, 900000000000000, */
/*   /\* 17 *\/ 0, 10000000000000000, 20000000000000000, 30000000000000000, 40000000000000000, 50000000000000000, 60000000000000000, 70000000000000000, 80000000000000000, 9000000000000000, */
/*   /\* 18 *\/ 0, 100000000000000000, 200000000000000000, 300000000000000000, 400000000000000000, 500000000000000000, 600000000000000000, 700000000000000000, 800000000000000000, 90000000000000000, */
                // 9223372036854775807
/*   /\* 19 *\/ 0, 1000000000000000000, 2000000000000000000, 3000000000000000000, 4000000000000000000, 5000000000000000000, 6000000000000000000, 7000000000000000000, 8000000000000000000, 900000000000000000, */
/*   /\* 20 *\/ 0, 10000000000000000000, 20000000000000000000, 30000000000000000000, 40000000000000000000, 50000000000000000000, 60000000000000000000, 70000000000000000000, 80000000000000000000, 9000000000000000000, */
/*   /\* 21 *\/ 0, 100000000000000000000, 200000000000000000000, 300000000000000000000, 400000000000000000000, 500000000000000000000, 600000000000000000000, 700000000000000000000, 800000000000000000000, 90000000000000000000, */
/* }; */

// 9,  223,372,036,  854,775,807 => 19 digits max int64

static const char shuffleMaps[/* 16 - 2 */ 17 * 32] __attribute__ ((aligned (32))) = {
  /* 0 digits */ 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
                 0, 0, 0, 0, 0, 0, 0, 0,
                 0, 0, 0, 0,
                 0, 0, 0, 0,
  /* 1 digits */ 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
  /* 2 digits */ 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
  /* 3 digits */ 2, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
  /* 4 digits */ 3, 2, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
  /* 5 digits */ 4, 3, 2, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
  /* 6 digits */ 5, 4, 3, 2, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
  /* 7 digits */ 6, 5, 4, 3, 2, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
  /* 8 digits */ 7, 6, 5, 4, 3, 2, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
  /* 9 digits */ 8, 7, 6, 5, 4, 3, 2, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
  /* 10digits */ 9, 8, 7, 6, 5, 4, 3, 2, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
  /* 11digits */10, 9, 8, 7, 6, 5, 4, 3, 2, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
  /* 12digits */11,10, 9, 8, 7, 6, 5, 4, 3, 2, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
  /* 13digits */12,11,10, 9, 8, 7, 6, 5, 4, 3, 2, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
  /* 14digits */13,12,11,10, 9, 8, 7, 6, 5, 4, 3, 2, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
  /* 15digits */14,13,12,11,10, 9, 8, 7, 6, 5, 4, 3, 2, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
  /* 16digits */15,14,13,12,11,10, 9, 8, 7, 6, 5, 4, 3, 2, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0
};

static __m256i _32_zeros;
static __m256i _32_nines;
static __m256i digitBases;

void initAvx2Int64Parser() {
  _32_zeros = _mm256_load_si256((__m256i*) BY32("00000000000000000000000000000000"));
  _32_nines = _mm256_load_si256((__m256i*) BY32("99999999999999999999999999999999"));
  digitBases = _mm256_set_epi8(70, 60, 50, 40, 30, 20, 10, 0,
                               70, 60, 50, 40, 30, 20, 10, 0,

                               70, 60, 50, 40, 30, 20, 10, 0,
                               70, 60, 50, 40, 30, 20, 10, 0);
}

int64_t avx2Int64Parser(const char* aligned) {
  m256_print_epi8("digit bases = ", digitBases);
  __m256i i = _mm256_load_si256((__m256i*) aligned);
  // __m256i* pi = &i;
  m256_print_epi8("i =           ", i);
  __m256i mask1 = _mm256_or_si256(_mm256_cmpgt_epi8(_32_zeros, i), _mm256_cmpgt_epi8(i, _32_nines));
  const int digitPrefix = __builtin_ctz(_mm256_movemask_epi8(mask1));
#ifdef DEBUG_AVX2_INT64_PARSER
  printf("digitPrefix = %d\n", digitPrefix);
#endif
  __m256i bin = _mm256_sub_epi8(i, _32_zeros);
  __m256i binReversed = _mm256_shuffle_epi8(bin, _mm256_load_si256((__m256i*) &shuffleMaps[digitPrefix * 32]));
  m256_print_epi8("bin =         ", bin);
  m256_print_epi8("bin reversed =", binReversed);
  m256_print_epi8("mask =        ", mask1);

  __m256i bcd32Vidx = _mm256_andnot_si256(mask1, _mm256_adds_epu8(binReversed, digitBases));
  m256_print_epi8("bcd32Vidx     ", bcd32Vidx);
  __m128i bcdFirst16DigitsVidx = _mm256_extracti128_si256(bcd32Vidx, 0);
  // __m128i bcdSecond16Digits = _mm256_extracti128_si256(bcd32Digits, 1);
  __m256i bcdFirst8 = _mm256_cvtepi8_epi32(bcdFirst16DigitsVidx);
  m256_print_epi32("bcdFirst8    ", bcdFirst8);
#ifdef DEBUG_AVX2_INT64_PARSER
  printf("%d %d %d \n", (int) posScaledDigits[1], (int) posScaledDigits[12], (int) posScaledDigits[23]);
#endif
  __m256i scaled8Digits = _mm256_i32gather_epi32((void*) posScaledDigits, bcdFirst8, 4);
  m256_print_epi32("scaled8Digits", scaled8Digits);

  // __m256i nextScaled8Digits = _mm256_i32gather_epi32((void*) &posScaledDigits[8*10], bcdSecond8, 1);
  // on hadd (horizontal add)
  // v'[0] = v[0] + v[1]
  // v'[1] = v[2] + v[3]
  // v'[4] = v[4] + v[5]
  // v'[5] = v[6] + v[7]
  // second hadd
  // v''[0] = v'[0] + v'[1]
  // v''[4] = v'[4] + v'[5]
  __m256i zero32 = _mm256_setzero_si256();
  __m256i twoInts = _mm256_hadd_epi32(_mm256_hadd_epi32(scaled8Digits, zero32),
                                      zero32);
  if (digitPrefix > 8) {
    __m128i bcdSecond4 = _mm_cvtepi8_epi32(_mm_srli_si128(bcdFirst16DigitsVidx, 8));
    m_print_epi32("bcdSecond4    ", bcdSecond4);
#ifdef DEBUG_AVX2_INT64_PARSER
    printf("posScaledDigitsLong %" PRId64 ";  %" PRId64 " ; %d\n" ,
           posScaledDigitsLong[1],
           posScaledDigitsLong[_mm_extract_epi32(bcdSecond4, 0)],
           _mm_extract_epi32(bcdSecond4, 0));
#endif
    __m256i scaled4Digits = _mm256_i32gather_epi64((void*) posScaledDigitsLong, bcdSecond4, 8);
    m256_print_epi64("scaled4Digits ", scaled4Digits);

    if (digitPrefix > 12) {
      __m128i bcdThird4 = _mm_cvtepi8_epi32(_mm_srli_si128(bcdFirst16DigitsVidx, 8 + 4));
      m_print_epi32("bcdThird4    ", bcdThird4);
      __m256i thridScaled4Digits = _mm256_i32gather_epi64((void*) posScaledDigitsLong, bcdThird4, 8);
      m256_print_epi64("thridScaled4Digits ", thridScaled4Digits);

      __m256i s2ndPlus3rdQuads = _mm256_add_epi64(scaled4Digits, thridScaled4Digits);

      __m128i f4d = _mm_add_epi64(_mm256_extracti128_si256(s2ndPlus3rdQuads, 0),
                                  _mm256_extracti128_si256(s2ndPlus3rdQuads, 1));
#ifdef DEBUG_AVX2_INT64_PARSER
      printf("%" PRId64 "\n%" PRId64 "\n%d\n%d\n",
             _mm_extract_epi64(f4d, 0),
             _mm_extract_epi64(f4d, 1),
             _mm256_extract_epi32(twoInts, 0),
             _mm256_extract_epi32(twoInts, 4));
#endif

      return
        _mm_extract_epi64(f4d, 0) +
        _mm_extract_epi64(f4d, 1) +
        _mm256_extract_epi32(twoInts, 0) +
        _mm256_extract_epi32(twoInts, 4);
    } else {
      __m128i f4d = _mm_add_epi64(_mm256_extracti128_si256(scaled4Digits, 0),
                                  _mm256_extracti128_si256(scaled4Digits, 1));

#ifdef DEBUG_AVX2_INT64_PARSER
      printf("%" PRId64 "\n%" PRId64 "\n%d\n%d\n",
             _mm_extract_epi64(f4d, 0),
             _mm_extract_epi64(f4d, 1),
             _mm256_extract_epi32(twoInts, 0),
             _mm256_extract_epi32(twoInts, 4));
#endif

      return
        _mm_extract_epi64(f4d, 0) +
        _mm_extract_epi64(f4d, 1) +
        _mm256_extract_epi32(twoInts, 0) +
        _mm256_extract_epi32(twoInts, 4);
    }
  } else {
    return _mm256_extract_epi32(twoInts, 0) + _mm256_extract_epi32(twoInts, 4);
  }
}

double duration(struct timeval* started, struct timeval* ended) {
  return 1e-6 * (ended->tv_sec * 1000000.0 + ended->tv_usec -
                 (started->tv_sec * 1000000.0 + started->tv_usec));
}
int main() {
  initAvx2Int64Parser();
  const char* number = BY32("1234567890123456\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0");
  const int n = 100*1000*1000 + 1;
  struct timeval started, ended;
  int64_t x = 0;

  gettimeofday(&started, NULL);
  for (int i = 0; i < n; ++i) {
    int64_t y;
    sscanf(number, "%" PRId64, &y);
    x ^= y;
  }
  gettimeofday(&ended, NULL);
  printf("sscanf:  rate: %18.2f; total: %10.4fs; %" PRId64 "\n",
         (double) n / duration(&started, &ended),
         duration(&started, &ended), x);
  x = 0;
  gettimeofday(&started, NULL);
  for (int i = 0; i < n; ++i) {
    x ^= avx2Int64Parser(number);
  }
  gettimeofday(&ended, NULL);
  printf("avx2pr:  rate: %18.2f; total: %10.4fs; %" PRId64 "\n",
         (double) n / duration(&started, &ended),
         duration(&started, &ended), x);
  return 0;
}
