#include <iostream>

template <typename K, typename V>
class MyMap {
 public:
  MyMap() { std::cout << "Generalization \n"; }
};

template <typename V>
class MyMap<int, V> {
 public:
  MyMap() { std::cout << "Partial template specialization\n"; }
};

template <>
class MyMap<int, double> {
 public:
  MyMap() { std::cout << "Total template specialization\n"; }
};

int main() {
  MyMap<double, double>();
  MyMap<int, int>();
  MyMap<int, double>();
}