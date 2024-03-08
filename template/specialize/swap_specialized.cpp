#include <iostream>
#include <utility>
#include <vector>
class WidgetImpl {
 private:
  int a_, b_, c_;
  std::vector<double> v_;
};

class Widget {
 public:
  Widget() = default;
  Widget(const Widget &rhs);
  Widget &operator=(const Widget &rhs) {
    *pImpl = *(rhs.pImpl);  // slow
    return *this;
  }
  // High efficiency swaps almost always base inner-type to operate.
  // Then, operation base inner-type never throw exception.
  void swap(Widget &w) noexcept {
    std::cout << "Widget::swap called\n";
    using std::swap;  // expose std version
    swap(pImpl, w.pImpl);
  }

 private:
  WidgetImpl *pImpl;
};

// In C++, function template(std::swap) can not be partial specialization.
// Only class templates can implement partial spec.
namespace std {
template <>
void swap<Widget>(Widget &l, Widget &r) {
  std::cout << "total template specialization\n";
  l.swap(r);
}
}  // namespace std

// But can implement a Partial-Like version by Function overloading.
// (if Widget is a class template)
// template<typename T>
// class Widget{...}
//
// But add new function define like overloading in std is forbidden.
// add new specialization is allowed.
/*
    namespace std {
    template <typename T>
    void swap(Widget<T> &l, Widget<T> &r) {
      l.swap(r);
    }
    }  // namespace std
*/
int main() {
  Widget w1{};
  Widget w2{};
  std::swap(w1, w2);
}