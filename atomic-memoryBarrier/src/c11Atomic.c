#include <stdatomic.h>

atomic_int atomic_counter;
int main() {
  // 初始化原子变量
  atomic_init(&atomic_counter, 0);

  // 原子加法
  atomic_fetch_add(&atomic_counter, 1);

  // 原子减法
  atomic_fetch_sub(&atomic_counter, 1);

  // 原子比较并交换
  int expected = 1;
  atomic_compare_exchange_strong(&atomic_counter, &expected, 2);
}
