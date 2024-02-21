#include <stdio.h>
#include <string.h>
#include <stdio.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include <errno.h>
#include <fcntl.h>
#include <sys/mman.h>
#include <stdlib.h>
#include <time.h>
#include <immintrin.h>

#define die(...) { fprintf(stderr, __VA_ARGS__); exit(1); }
#define debug(...) { fprintf(stderr, __VA_ARGS__); }
//#define debug(...)

static char stopChars[32] __attribute__((aligned(32))) = "\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0";

int countCharsSeq(const char** input, int inputLen, const char* stopChars) {
  int result = 0;
  while (inputLen--) {
    for (const char* sc = stopChars; *sc; ++sc) {
      if (*sc == **input) {
        result++;
        break;
      }
    }
    (*input)++;
  }
  return result;
}

int countChars(const char* input, int inputLen, const char* stopChars) {
  const __m256i zero = _mm256_set_epi64x(0, 0, 0, 0);
  const __m256i stop_chars = _mm256_load_si256((__m256i*) stopChars);
  const int skipToAlign = 0x30 - (long) input & 0x1f;
  const char* const originInput = input;
  debug("Origin input addr %p and len %d\n", input, inputLen);
  int sseCycles = (inputLen - skipToAlign) >> 5;
  const int headResult = countCharsSeq(&input, skipToAlign, stopChars);
  __m256i result256 = _mm256_set_epi64x(0, 0, 0, 0);
  debug("Skip %03d bytes for alignment; found %03d stop chars; Aligned input addr %p\n", skipToAlign, headResult, input);
  debug("SSE cycles %03d\n", sseCycles);
 w:
  for (const char* endOfBatchInput = input + sseCycles*32; input < endOfBatchInput; input += 32) {
    __m256i input_a = _mm256_load_si256((__m256i*) input);

    __m256i input_b = _mm256_load_si256((__m256i*) input);
    input += 32;
    // debug("SSE cycles %03d; input %p\n", sseCycles, input);
    // const int bytesToSkipToNextStopChar = _mm_cmpistri(stop_chars, input_a, _SIDD_UBYTE_OPS | _SIDD_CMP_EQUAL_ANY);
    __m256i bits =  _mm256_popcnt_epi32(_mm256_cmpeq_epi8(input_a, stop_chars));
    result256 = _mm256_add_epi32(result256, _mm256_srli_epi32(bits, 3));

    // __m256i _mm256_popcnt_epi32
    // how to sum
    // __m256i _mm256_castsi128_si256 (__m128i a)
    // __m128i _mm256_castsi256_si128 (__m256i a)
    // __m256i _mm256_cmpeq_epi8 (__m256i a, __m256i b)
    // __m256i _mm256_load_si256 (__m256i const * mem_addr)
    // int _mm256_movemask_epi8 (__m256i a)
    // __m256i _mm256_set_epi64x (__int64 e3, __int64 e2, __int64 e1, __int64 e0)
    // lzcnt unsigned int _lzcnt_u32 (unsigned int a)
  }
  const int avxResult = _mm256_extract_epi32(result256, 0) +
    _mm256_extract_epi32(result256, 1) +
    _mm256_extract_epi32(result256, 2) +
    _mm256_extract_epi32(result256, 3) +
    _mm256_extract_epi32(result256, 4) +
    _mm256_extract_epi32(result256, 5) +
    _mm256_extract_epi32(result256, 6) +
    _mm256_extract_epi32(result256, 7);
  const int afterCycles = (originInput + inputLen) - input;
  const int tailResult = countCharsSeq(&input, afterCycles, stopChars);
  debug("Bytes in incomplete cycle %03d; Head: %03d; Avx %03d; Tail: %03d\n", afterCycles, headResult, avxResult, tailResult);
  return headResult + avxResult + tailResult;
}


struct stat fdStat;
const char* endOfMem = 0;
const char* startOfMem = 0;

int main(int argc, const char** argv) {
  if (argc != 3) {
    die("Expected args: file and charts to count\n");
  }
  const int fd = open(argv[1], O_RDONLY);
  if (fd < 0) die("Cannot open file %s (errno %d)\n", argv[1], errno);
  if (fstat(fd, &fdStat) < 0) die("Failed to get FILE stat\n");
  startOfMem = mmap(NULL, fdStat.st_size, PROT_READ, MAP_PRIVATE, fd, 0);
  if (startOfMem == (void*) -1) die("Failed to mmap file (errno %d)\n", errno);
  endOfMem = startOfMem + fdStat.st_size;
  if (endOfMem == startOfMem) exit(0);

  const int lenOfStopChars = strlen(argv[2]);
  if (lenOfStopChars != 1) {
    die("Length of stop characters is not 1: %d\n", lenOfStopChars);
  }
  for (int i = 0; i < sizeof(stopChars); ++i) {
    stopChars[i] = *argv[2];
  }
  const int n = countChars(startOfMem, fdStat.st_size, stopChars);
  printf("%d\n", n);
  return 0;
}
