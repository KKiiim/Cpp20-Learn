#include <cstdio>

#include "v.hpp"

int main() {
  VDerived v;
  v.set();  // actually, set() had not be inlined.

  printf("%d \n", v.d);
}