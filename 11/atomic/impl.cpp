#include <atomic>
std::atomic_int acnt;
int main() { ++acnt; }

/*
acnt:
        .zero   4
main:
        push    rbp
        mov     rbp, rsp
        mov     edi, OFFstd::__atomic_base<int>::operator++()SET FLAT:acnt
        call    
        mov     eax, 0
        pop     rbp
        ret
*/