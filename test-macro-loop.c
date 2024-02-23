#include <stdio.h>
#include "macroloop.h"

#define MYOP(x, y, ...) printf( CAT(x, y), __VA_ARGS__)

int main() {
  const int xyz = 0;
  M(EVAL(WHILE(PRED, MYOP, x, y, z)));
  return xyz;
}
