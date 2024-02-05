/*
    std::mutex  since C++11
    This example shows how a mutex can be used to protect an std::map shared
   between two threads.
*/
#include <chrono>
#include <iostream>
#include <map>
#include <mutex>
#include <string>
#include <thread>

std::map<std::string, std::string> g_pages;
std::mutex g_pages_mutex;

void save_page(const std::string &url) {
  // simulate a long page fetch
  std::this_thread::sleep_for(std::chrono::seconds(1));
  std::string result = "fake content";
  /*
    guard is a stack object with lock() in constructor and a unlock in
    destructor. stack object will dead after this function call.
  */
  std::lock_guard<std::mutex> guard(g_pages_mutex);
  g_pages[url] = result;
}

int main() {
  std::thread t1(save_page, "http://foo");
  std::thread t2(save_page, "http://bar");
  t1.join();
  t2.join();

  // safe to access g_pages without lock now, as the threads are joined
  for (const auto &pair : g_pages)
    std::cout << pair.first << " => " << pair.second << '\n';
}