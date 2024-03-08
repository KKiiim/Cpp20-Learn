template <typename T>
concept C1 = sizeof(T) != sizeof(int);

template <C1 T>
struct S1 {};

template <C1 T>
using Ptr = T*;

S1<int>* p;  // error: constraints not satisfied
Ptr<int> p;  // error: constraints not satisfied

template <typename T>
struct S2 {
  Ptr<int> x;
};  // error, no diagnostic required

template <typename T>
struct S3 {
  Ptr<T> x;
};  // OK, satisfaction is not required

S3<int> x;  // error: constraints not satisfied

template <template <C1 T> class X>
struct S4 {
  X<int> x;  // error, no diagnostic required
};

template <typename T>
concept C2 = sizeof(T) == 1;

template <C2 T>
struct S {};

template struct S<char[2]>;  // error: constraints not satisfied
template <>
struct S<char[2]> {};  // error: constraints not satisfied