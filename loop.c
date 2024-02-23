int foo(int x) {
  int r = 1;
#pragma GCC unroll 3
  for (int i = x; i < 3; ++i) {
    r = r + i;
  }
  return r;
}
