#include <stdio.h>
#include <immintrin.h>


int main() {
  __m256i hello;
  __m256d a = _mm256_set_pd(1.0, 2.0, 3.0, 4.0);

  __m256d b = a;

  __m256d c = _mm256_add_pd(a, b);

  __attribute__ ((aligned (32))) double output[4];
  _mm256_store_pd(output, c);

  printf("%f %f %f %f\n", output[0], output[1], output[2], output[3]);


  return 0;
}
