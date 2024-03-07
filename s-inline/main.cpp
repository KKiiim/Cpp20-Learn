#include "t.hpp"

int main(){
    Test t;
    t.set();
}
// error
/**
/usr/bin/ld: /tmp/cc8Mw2Ko.o: in function `main':
main.cpp:(.text+0x23): undefined reference to `Test::set()'
collect2: error: ld returned 1 exit status
*/