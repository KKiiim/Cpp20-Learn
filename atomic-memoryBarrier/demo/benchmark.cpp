#include <chrono>
#include <iostream>
#include <set>
#include <thread>

std::set<int> test_set;
void test() {
  for (int i = 0; i < 1000000; ++i) {
    test_set.insert(i);
  }
}

auto test_func_duration = [](auto &&func, auto &&...params) {
  auto now = std::chrono::system_clock::now();
  std::forward<decltype(func)>(func)(std::forward<decltype(params)>(params)...);
  auto dur = std::chrono::system_clock::now() - now;

  std::cout
      << "duration ms: "
      << std::chrono::duration_cast<std::chrono::milliseconds>(dur).count()
      << std::endl;
};

class Benchmark {
 public:
  Benchmark() { m_begin = std::chrono::system_clock::now(); }
  ~Benchmark() {
    auto dur = std::chrono::system_clock::now() - m_begin;
    std::cout
        << "duration ms: "
        << std::chrono::duration_cast<std::chrono::milliseconds>(dur).count()
        << std::endl;
  }
  Benchmark(const Benchmark &) = delete;
  Benchmark &operator=(const Benchmark &) = delete;

 private:
  using time_point = decltype(std::chrono::system_clock::now());
  time_point m_begin;
};

int main() {
  // 方法1
  // auto now = std::chrono::system_clock::now();
  // test();
  // auto dur = std::chrono::system_clock::now() - now;

  // std::cout << "duration ms: " <<
  // std::chrono::duration_cast<std::chrono::milliseconds>(dur).count() <<
  // std::endl;

  // 方法2
  // test_func_duration(test);

  // 方法3
  {
    Benchmark bm;
    test();
  }

  return 0;
}
