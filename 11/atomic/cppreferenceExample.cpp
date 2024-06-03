#include <atomic>
#include <iostream>
#include <thread>
#include <vector>

std::atomic_int atomicCnt = 0;
int normalCnt = 0;
int asmLocakCnt = 0;

#include <iostream>

void asmImpl() {
  // The inline assembly syntax for GCC/Clang is as follows:
  // asm ( assembler template
  //     : output operands                  /* optional */
  //     : input operands                   /* optional */
  //     : list of clobbered registers      /* optional */
  //     );

  asm volatile(
      "lock xaddl %1, %0;"  // The 'l' suffix specifies that the
                            // operation is on a 32-bit integer.
      : "=m"(asmLocakCnt)   // Output operand: memory(m) location of
                           // 'asmLocakCnt' will be writen(=).
      : "r"(1), "m"(asmLocakCnt)  // The C++ variable asmLocakCnt is linked to
                                  // %1 in the assembly instruction.
      : "memory", "cc"            // Clobber list: flags that the memory and
                                  // condition codes may be modified.
  );
}

void f() {
  for (int n = 0; n < 10000; ++n) {
    ++atomicCnt;
    ++normalCnt;
    // Note: for this example, relaxed memory order
    // is sufficient, e.g. acnt.fetch_add(1, std::memory_order_relaxed);
    asmImpl();
  }
}

int main() {
  {
    std::vector<std::jthread> pool;  // c++ 20
    for (int n = 0; n < 10; ++n) pool.emplace_back(f);
  }

  std::cout << "The atomic counter is " << atomicCnt << '\n'
            << "The non-atomic counter is " << normalCnt << '\n'
            << "The asmLock counter is " << asmLocakCnt << std::endl;
}