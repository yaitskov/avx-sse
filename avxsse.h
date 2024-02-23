#ifndef AVXSSE_H_
#define AVXSSE_H_

#include <immintrin.h>
#include <inttypes.h>
#include "immediate.h"

#define ALIGNED_STRING(a,S)  (struct { _Alignas(a) char s[sizeof S]; }){ S }.s
#define BY16(S) ALIGNED_STRING(16,S)
#define BY32(S) ALIGNED_STRING(32,S)

#ifdef DEBUG_AVX2_INT64_PARSER

void m256_print_epi32(const char* p, __m256i x) {
  printf("%s  %6d %6d %6d %6d %6d %6d %6d %6d\n",
         p,
         _mm256_extract_epi32(x, 0),
         _mm256_extract_epi32(x, 1),
         _mm256_extract_epi32(x, 2),
         _mm256_extract_epi32(x, 3),
         _mm256_extract_epi32(x, 4),
         _mm256_extract_epi32(x, 5),
         _mm256_extract_epi32(x, 6),
         _mm256_extract_epi32(x, 7)
         );
}

void m256_print_epi64(const char* p, __m256i x) {
  printf("%s  %" PRId64 " %" PRId64 " %" PRId64 " %" PRId64 "\n",
         p,
         _mm256_extract_epi64(x, 0),
         _mm256_extract_epi64(x, 1),
         _mm256_extract_epi64(x, 2),
         _mm256_extract_epi64(x, 3)
         );
}

void m_print_epi32(const char* p, __m128i x) {
  printf("%s  %" PRId32 " %" PRId32 " %" PRId32 " %" PRId32 "\n",
         // printf("%s  %6d %6d %6d %6d\n",
         p,
         _mm_extract_epi32(x, 0),
         _mm_extract_epi32(x, 1),
         _mm_extract_epi32(x, 2),
         _mm_extract_epi32(x, 3)
         );
}

void m256_print_epi8(const char* p, __m256i x) {
  printf("%s  %02x %02x %02x %02x %02x %02x %02x %02x %02x %02x %02x %02x %02x %02x %02x %02x %02x %02x %02x %02x %02x %02x %02x %02x %02x %02x %02x %02x %02x %02x %02x %02x \n",
         p,
         _mm256_extract_epi8(x, 0),
         _mm256_extract_epi8(x, 1),
         _mm256_extract_epi8(x, 2),
         _mm256_extract_epi8(x, 3),
         _mm256_extract_epi8(x, 4),
         _mm256_extract_epi8(x, 5),
         _mm256_extract_epi8(x, 6),
         _mm256_extract_epi8(x, 7),

         _mm256_extract_epi8(x, 8),
         _mm256_extract_epi8(x, 9),
         _mm256_extract_epi8(x, 10),
         _mm256_extract_epi8(x, 11),
         _mm256_extract_epi8(x, 12),
         _mm256_extract_epi8(x, 13),
         _mm256_extract_epi8(x, 14),
         _mm256_extract_epi8(x, 15),

         _mm256_extract_epi8(x, 16),
         _mm256_extract_epi8(x, 17),
         _mm256_extract_epi8(x, 18),
         _mm256_extract_epi8(x, 19),
         _mm256_extract_epi8(x, 20),
         _mm256_extract_epi8(x, 21),
         _mm256_extract_epi8(x, 22),
         _mm256_extract_epi8(x, 23),

         _mm256_extract_epi8(x, 24),
         _mm256_extract_epi8(x, 25),
         _mm256_extract_epi8(x, 26),
         _mm256_extract_epi8(x, 27),
         _mm256_extract_epi8(x, 28),
         _mm256_extract_epi8(x, 29),
         _mm256_extract_epi8(x, 30),
         _mm256_extract_epi8(x, 31)
         );
}

#else

#define m256_print_epi32(p,x)
#define m256_print_epi64(p,x)
#define m_print_epi32(p,x)
#define m256_print_epi8(p,x)
#endif

#endif
