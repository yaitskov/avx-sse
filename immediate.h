#ifndef IMMEDIATE_H
#define IMMEDIATE_H

#define immediate(f, bn, n, ...)                                        \
  switch (n) {                                                          \
  case 1: f(bn, 1 __VA_ARGS__); break;                                  \
  case 2: f(bn, 2 __VA_ARGS__); break;                                  \
  case 3: f(bn, 3 __VA_ARGS__); break;                                  \
  case 4: f(bn, 4 __VA_ARGS__); break;                                  \
  case 5: f(bn, 5 __VA_ARGS__); break;                                  \
  case 6: f(bn, 6 __VA_ARGS__); break;                                  \
  case 7: f(bn, 7 __VA_ARGS__); break;                                  \
  case 8: f(bn, 8 __VA_ARGS__); break;                                  \
  case 9: f(bn, 9 __VA_ARGS__); break;                                  \
  case 10: f(bn, 10 __VA_ARGS__); break;                                \
  case 11: f(bn, 11 __VA_ARGS__); break;                                \
  case 12: f(bn, 12 __VA_ARGS__); break;                                \
  case 13: f(bn, 13 __VA_ARGS__); break;                                \
  case 14: f(bn, 14 __VA_ARGS__); break;                                \
  case 15: f(bn, 15 __VA_ARGS__); break;                                \
  case 16: f(bn, 16 __VA_ARGS__); break;                                \
  case 17: f(bn, 17 __VA_ARGS__); break;                                \
  default:                                                              \
  fprintf(stderr, "Panic no case for n=%d in ##f\n", n); exit(1);       \
  }

#endif
