#include <coroutine>
#include <iostream>

/*
  g++ --std=c++20 coro.cc
*/

struct Awaiter {
    //  the three special methods that are called as part of a co_await
  bool await_ready() {
    std::cout << "await ready or not" << std::endl;
    return false;
  }

  void await_resume() {
    std::cout << "await resumed" << std::endl;
    // Customizable implementation and return type as {co_await Awaiter}'s return.
  }

  void await_suspend(std::coroutine_handle<> h) {
    std::cout << "await suspended" << std::endl;
    // theoretically, here, can jump to other coroutine.
    // in this return 'void' feature, once execute coro suspend, controler will jump to caller(main).
  }
};

struct Promise {
    // one promise(coroutine function return value type) object must define promise_type.
  struct promise_type {

    // once this-Promise-object constructed, a object as return object will be created. 
    // after get_return_object(), when this coroutine be suspended, the return-object will be returned to caller.

    // auto get_return_object() noexcept {
    //   std::cout << "get return object" << std::endl;
    //   return Promise();
    // }

   Promise get_return_object(){
    std::cout << "get return object" << std::endl;
    return {std::coroutine_handle<promise_type>::from_promise(*this)};
   }// can return this(itself) for caller/resumer to continue me later.


    auto initial_suspend() noexcept {
      std::cout << "initial suspend, return never" << std::endl;
      return std::suspend_never{};
    }

    // std::suspend_never initial_suspend() noexcept {return {};}

    auto final_suspend() noexcept {
      std::cout << "final suspend, return never" << std::endl;
      return std::suspend_never{};
    }

    void unhandled_exception() {
      std::cout << "unhandle exception" << std::endl;
      std::terminate();
    }

    void return_void() {
      std::cout << "return void" << std::endl;
      return;
    }
  };

  // coroutine controler.
  // _h.resume() or _h() to continue the corresponding coroutine.
  std::coroutine_handle<promise_type> _h;
};

// once coroutine function begin, a Promise object will be constructed implicitly.
// the promise-object is used to control the operation of coroutine and data exchangment between out-in this coroutine.
Promise CoroutineFunc() {
  std::cout << "before co_await" << std::endl;
  co_await Awaiter();
  // in object Awaiter, there is a function await_ready to control whether co_wait need to suspend current coro.
  std::cout << "after co_await" << std::endl;
}

int main() {
  std::cout << "main() start" << std::endl;
  auto ret = CoroutineFunc();
  // ret._h();
  std::cout << "main() exit" << std::endl;
}

/*
main() start
get return object
initial suspend, return never
before co_await
await ready or not
await resumed
after co_await
return void
final suspend, return never
main() exit
*/