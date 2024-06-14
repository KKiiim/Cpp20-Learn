#include <atomic>
#include <iostream>
#include <thread>

std::atomic<bool> data_ready(false);
int non_atomic_data = 0;

void writer_thread() {
  non_atomic_data = 42;  // Set non-atomic data.

  // Insert a memory barrier to ensure that the write to non_atomic_data
  // is visible to other threads before the write to data_ready.
  std::atomic_thread_fence(std::memory_order_release);

  data_ready.store(true,
                   std::memory_order_relaxed);  // Notify that data is ready.
}

void reader_thread() {
  while (!data_ready.load(std::memory_order_relaxed)) {
    // Wait for data to become ready.
    std::this_thread::yield();
  }

  // Insert a memory barrier to ensure that all reads and writes before this
  // point are completed before any reads and writes after this point.
  std::atomic_thread_fence(std::memory_order_acquire);

  // It's now safe to read non_atomic_data.
  std::cout << "Non-atomic data: " << non_atomic_data << std::endl;
}

int main() {
  std::thread writer(writer_thread);
  std::thread reader(reader_thread);

  writer.join();
  reader.join();

  return 0;
}