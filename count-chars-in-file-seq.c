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
#define debug(...)


int countCharsSeq(const char* input, int inputLen) {
  int result = 0;
  while (inputLen--) {
    if ('\n' == *input++) {
        result++;
      }
  }
  return result;
}

struct stat fdStat;
const char* endOfMem = 0;
const char* startOfMem = 0;

int main(int argc, const char** argv) {
  if (argc != 2) {
    die("Expected args: file\n");
  }
  const int fd = open(argv[1], O_RDONLY);
  if (fd < 0) die("Cannot open file %s (errno %d)\n", argv[1], errno);
  if (fstat(fd, &fdStat) < 0) die("Failed to get FILE stat\n");
  startOfMem = mmap(NULL, fdStat.st_size, PROT_READ, MAP_PRIVATE, fd, 0);
  if (startOfMem == (void*) -1) die("Failed to mmap file (errno %d)\n", errno);
  endOfMem = startOfMem + fdStat.st_size;
  if (endOfMem == startOfMem) exit(0);
  const int n = countCharsSeq(startOfMem, fdStat.st_size);
  printf("%d\n", n);
  return 0;
}
