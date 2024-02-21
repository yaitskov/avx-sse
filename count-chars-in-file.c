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
// #define debug(...) { fprintf(stderr, __VA_ARGS__); }
#define debug(...)

static char stopChars[16] __attribute__((aligned(16))) = "\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0";

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

// #define AVX

int countChars(const char* input, int inputLen, const char* stopChars) {
  const __m128i zero = _mm_setzero_si128();
  const __m128i stop_chars = _mm_loadu_si128((__m128i*) stopChars);
  const int skipToAlign = 0x10 - (long) input & 0xf;
  const char* const originInput = input;
  debug("Origin input addr %p and len %d\n", input, inputLen);
  int sseCycles = (inputLen - skipToAlign) >> 4;
  int result = countCharsSeq(&input, skipToAlign, stopChars);
  debug("Skip %03d bytes for alignment; found %03d stop chars; Aligned input addr %p\n", skipToAlign, result, input);
  debug("SSE cycles %03d\n", sseCycles);
 w:
  while (sseCycles--) {
    __m128i input_a = _mm_loadu_si128((__m128i*) input);
    debug("SSE cycles %03d; input %p\n", sseCycles, input);
    for (;;) {
      const int bytesToSkipToNextStopChar = _mm_cmpistri(stop_chars, input_a, _SIDD_UBYTE_OPS | _SIDD_CMP_EQUAL_ANY);
#ifdef AVX
      // __m256i _mm256_cmpeq_epi8 (__m256i a, __m256i b)
      // __m256i _mm256_load_si256 (__m256i const * mem_addr)
      // int _mm256_movemask_epi8 (__m256i a)
      input_a = _mm256_castsi256_si128(_mm256_bslli_epi128(_mm256_castsi128_si256(input_a),  bytesToSkipToNextStopChar));
#else
      switch (bytesToSkipToNextStopChar) {
      case 16:
        input += 16;
        goto w;
      case 0:
        // __m256i _mm256_castsi128_si256 (__m128i a)
        // __m128i _mm256_castsi256_si128 (__m256i a)
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
        debug("Skip more that 16 bytes %d\n", bytesToSkipToNextStopChar);
        return -1;
      }
#endif
      ++result;
      debug("Skip %d bytes\n", bytesToSkipToNextStopChar);
    }
  }
  const int afterCycles = (originInput + inputLen) - input;
  debug("Bytes in incomplete cycle %03d; Found stop bytes %03d; Aligned input addr %p\n", afterCycles, result, input);
  return result + countCharsSeq(&input, afterCycles, stopChars);
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
  if (lenOfStopChars > 16) {
    die("Length of stop characters is longer than 16: %d\n", lenOfStopChars);
  }
  if (!lenOfStopChars) {
    die("Stop characters string is empty\n");
  }
  memcpy(stopChars, argv[2], lenOfStopChars);
  const int n = countChars(startOfMem, fdStat.st_size, stopChars);
  printf("%d\n", n);
  return 0;
}
