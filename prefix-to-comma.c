#include <stdio.h>
#include <immintrin.h>

  //This seems to be our dictionary
static const unsigned char input[32] __attribute__((aligned(16))) = "123456\n,9,01234,";

//This seems to be the one that gives the index
static const unsigned char stopChars[16] __attribute__((aligned(16))) = ",\n";

#define simd_compare_string_mode (_SIDD_UBYTE_OPS | _SIDD_CMP_EQUAL_ANY) // | _SIDD_NEGATIVE_POLARITY | _SIDD_BIT_MASK)

// in case of stop character appeares multiple times in same block
// in should be reset
// int _mm_extract_epi8 (__m128i a, const int imm8) is for read only
// __m128i _mm_insert_epi8 (__m128i a, int i, const int imm8)
// __m128i _mm_andnot_si128 (__m128i a, __m128i b)
// __m128i _mm_bslli_si128 (__m128i a, int imm8)
// __m128i _mm_bsrli_si128 (__m128i a, int imm8)
// __m128i _mm_xor_si128 (__m128i a, __m128i b)
// __m128i _mm_blendv_epi8 (__m128i a, __m128i b, __m128i mask)
// __m128i _mm_load_si128 (__m128i const* mem_addr)
// __m128i _mm_loadu_epi8 (void const* mem_addr)

// __m128i _mm_setzero_si128 ()
// __m128i _mm_bslli_si128 (__m128i a, int imm8)

/* xor gives 0s
 * not gives 1s
 * shift left by 8 bytes
 * not gives 0xff
 * shift left by 16 - position
 * andnot
 */

int countChars(const char* input, int inputLen, const char* theChars) {

}

int main() {
  const __m128i zero = _mm_setzero_si128();
  const __m128i stop_chars = _mm_loadu_si128((__m128i*) stopChars);
  __m128i input_a = _mm_loadu_si128((__m128i*) input);
  for (;;) {
    const int result = _mm_cmpistri(stop_chars, input_a, _SIDD_UBYTE_OPS | _SIDD_CMP_EQUAL_ANY);
    // input_a = _mm_insert_epi8(input_a, 0, result);
    switch (result) {
    case 16:
      printf("End of array\n");
      return 0;
    case 0:
      input_a = _mm_bsrli_si128(input_a, 1);
      break;
    case 1:
      input_a = _mm_bsrli_si128(input_a, 2);
      break;
    case 2:
      input_a = _mm_bsrli_si128(input_a, 3);
      break;
    case 3:
      input_a = _mm_bsrli_si128(input_a, 4);
      break;
    case 4:
      input_a = _mm_bsrli_si128(input_a, 5);
      break;
    case 5:
      input_a = _mm_bsrli_si128(input_a, 6);
      break;
    case 6:
      input_a = _mm_bsrli_si128(input_a, 7);
      break;
    case 7:
      input_a = _mm_bsrli_si128(input_a, 8);
      break;
    case 8:
      input_a = _mm_bsrli_si128(input_a, 9);
      break;
    case 9:
      input_a = _mm_bsrli_si128(input_a,10);
      break;
    case 10:
      input_a = _mm_bsrli_si128(input_a,11);
      break;
    case 11:
      input_a = _mm_bsrli_si128(input_a,12);
      break;
    case 12:
      input_a = _mm_bsrli_si128(input_a,13);
      break;
    case 13:
      input_a = _mm_bsrli_si128(input_a,14);
      break;
    case 14:
      input_a = _mm_bsrli_si128(input_a,15);
      break;
    case 15:
      input_a = _mm_bsrli_si128(input_a, 16);
      break;
    default:
      printf("Skip more that 16 bytes %d\n", result);
      return -1;
    }
    printf("Skip %d bytes\n", result);
  }

  return 0;
}
