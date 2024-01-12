#include <array>
constexpr int foo(int i){
    return i+5;
}
int main(){
    int i = 10;
    std::array<int, foo(5)> arr1;// OK

    foo(i);                        // OK
    const int result = foo(i);      // run-time constant
    constexpr int result2 = foo(i); // til run-time constant

    std::array<int, foo(i)> arr2;   // Error

    const int j = 10;
    std::array<int, foo(j)> arr3;  // OK
    constexpr int result3 = foo(j); // compile-time constant

    std::array<int, result> arr4;
    std::array<int, result2> arr5;

}
/**
const:  indicates that the variable is immutable, but it does not specify whether the variable is a compile-time or run-time constant
constexpr: can only be a compile-time constant.
*/