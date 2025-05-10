/**
########################################################################
#
# Copyright (c) 2025 xx.com, Inc. All Rights Reserved
#
########################################################################
# Author : xuechengyun
# E-mail : xuechengyun@gmail.com
# Date   : 2025/05/10 18:43:40
# Desc   : 第 7 章 并行与并发
########################################################################
*/
#include "cpp_utils/util.h"
#include <cassert>
#include <chrono>
#include <condition_variable>
#include <future>
#include <mutex>
#include <print>
#include <queue>
#include <thread>

// 7.1 并行基础
void run_thread() {
  PRINT_CURRENT_FUNCTION_NAME;
  std::thread t([]() {
    std::println("hello");
  });
  std::println("thread_id: {}", t.get_id());
  t.join();
  std::println();
}

// 7.2 互斥量与临界区
int v = 1;
void critical_section(int change_v) {
  static std::mutex mtx;
  std::lock_guard<std::mutex> lock(mtx);
  v = change_v;
}

void critical_section_2(int change_v) {
  static std::mutex mtx;
  std::unique_lock<std::mutex> lock(mtx);
  v = change_v;
  lock.unlock();

  lock.lock();
  v += 1;
}
void run_mutex() {
  PRINT_CURRENT_FUNCTION_NAME;
  std::thread t1(critical_section, 2);
  std::thread t2(critical_section, 3);
  t1.join();
  t2.join();
  assert(v == 2 || v == 3);

  v = 1;
  std::thread t3(critical_section, 4);
  std::thread t4(critical_section, 5);
  t3.join();
  t4.join();
  // t3: 4  t3: 5   t4: 5  t4: 6
  // t3: 4  t4: 5   t3: 6  t4: 7
  // t3: 4  t4: 5   t4: 6  t3: 7

  // t4: 5  t4: 6   t3: 4  t3: 5
  // t4: 5  t3: 4   t4: 5  t3: 6
  // t4: 5  t3: 4   t3: 5  t4: 6
  assert(v == 5 || v == 6 || v == 7);
  std::println();
}

// 7.3 期物(future)
void run_future() {
  PRINT_CURRENT_FUNCTION_NAME;
  std::packaged_task<int()> task([]() { return 7; });
  std::future<int> result = task.get_future();
  std::thread(std::move(task)).detach();
  result.wait();
  std::println("future result is: {}", result.get());
  std::println();
}


// 7.4 条件变量
void run_condition_variable() {
  std::queue<int> produced_nums;
  std::mutex mtx;
  std::condition_variable cv;
  bool notified = false;
  int counter{0};
  int N{10};
  auto producer = [&]() {
    for (; counter < N; ++counter) {
      std::this_thread::sleep_for(std::chrono::milliseconds(900));
      std::unique_lock<std::mutex> lock(mtx);
      std::println("producing {}", counter);
      produced_nums.push(counter);
      notified = true;
      cv.notify_all();
    }
  };
  auto consumer = [&]() {
    while (!(counter == N && produced_nums.empty())) {
      std::unique_lock<std::mutex> lock(mtx);
      while (!notified) {
        cv.wait(lock);
      }
      lock.unlock();
      std::this_thread::sleep_for(std::chrono::milliseconds(1000));
      lock.lock();
      while (!produced_nums.empty()) {
        std::println("consuming {}", produced_nums.front());
        produced_nums.pop();
      }
      notified = false;
    }
  };
  std::thread p(producer);
  std::thread cs[2];
  for (auto i = 0; i < 2; ++i) {
    cs[i] = std::thread(consumer);
  }
  p.join();
  for (auto i = 0;i < 2; ++i) {
    cs[i].join();
  }
}

// 7.5 原子操作与内存模型
void run_memory_order() {
  int a{0};
  volatile int flag{0};
  std::thread t1([&]() {
    while (flag != 1);

    int b = a;
    std::println("b: {}", b);
  });
  std::thread t2([&]() {
    a = 5;
    flag = 1;
  });
  t1.join();
  t2.join();
}

// 原子操作
std::atomic<int> count{0};
void run_atomic() {
  std::thread t1([]() {
    count.fetch_add(1);
  });
  std::thread t2([]() {
    ++count;
    count += 1;
  });
  t1.join();
  t2.join();
}

int main() {
  run_thread();
  run_mutex();
  run_future();
  // run_condition_variable();
  run_memory_order();
  run_atomic();
  return 0;
}



