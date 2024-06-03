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

// memory_order_seq_cst
// On x86, the LOCK prefix on atomic instructions inherently provides seq_cst
// ordering, so no additional barriers are needed. On other architectures, like
// ARM, explicit memory barrier instructions may be required.

// memory_order_relaxed
// only Only atomicity is guaranteed; no synchronization or ordering

// memory_order_acquire
// Operations after the atomic operation in program order cannot be reordered
// before it.