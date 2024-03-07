#include <condition_variable>
#include <deque>
#include <iostream>
#include <mutex>
#include <thread>

std::mutex g_mtx;
std::condition_variable g_cond;
std::deque<int> g_deque;

void consumeFunc(int id) {
  while (true) {
    std::unique_lock<std::mutex> lck(g_mtx);
    // resolve spurious wakeup
    while (g_deque.empty()) {
      g_cond.wait(lck);
    }
    int aa = g_deque.back();
    std::cout << "thread " << id << "  " << aa << '\n';
    g_deque.pop_back();
    lck.unlock();
  }
}

class Produce {
 public:
  void static produceFunc() {
    unsigned int productions = 0;
    while (true) {
      std::unique_lock<std::mutex> lck(g_mtx);
      if (g_deque.empty()) {
        g_deque.push_back(productions++);
        g_cond.notify_all();
      }
      lck.unlock();
      std::this_thread::sleep_for(std::chrono::seconds(1));
    }
  }
};

int main() {
  std::thread consumeThread[10];

  for (int i = 0; i < 10; ++i) consumeThread[i] = std::thread(consumeFunc, i);

  std::thread produThread;

  produThread = std::thread(Produce::produceFunc);

  for (int i = 0; i < 10; ++i) {
    if (consumeThread[i].joinable()) {
      consumeThread[i].join();
    }
  }

  if (produThread.joinable()) {
    produThread.join();
  }

  return 0;
}