#include "thread_pool.h"

ThreadPool::ThreadPool(uint32_t thread_count) : thread_count_(thread_count) {
  Init();
}

ThreadPool::~ThreadPool() {
  {
    std::scoped_lock lock(mtx_);
    running_ = false;
    task_available_cv_.notify_all();
  }
  for (auto& thread : threads_) thread.join();
}

void ThreadPool::WaitAll() {
  auto cond_func = [&] {
    return queue_.empty() && task_ids_ == completed_tasks_;
  };

  waiting_ = true;
  std::unique_lock<std::mutex> lock(mtx_);
  while (!cond_func())  // Spurious awake handle
    task_cv_.wait(lock, cond_func);
  waiting_ = false;
}

void ThreadPool::Init() {
  running_ = true;
  threads_.reserve(thread_count_);
  for (uint32_t i = 0; i < thread_count_; ++i)
    threads_.emplace_back(&ThreadPool::Worker, this);
}

void ThreadPool::Worker() {
  auto cond_func = [&] { return !queue_.empty() || !running_; };

  while (running_) {
    std::unique_lock<std::mutex> lock(mtx_);

    while (!cond_func())  // Spurious awake handle
      task_available_cv_.wait(lock, cond_func);

    if (queue_.empty()) continue;

    auto [task, ID] = std::move(queue_.front());
    queue_.pop();
    lock.unlock();

    task();

    lock.lock();
    completed_task_ids_.insert(ID);
    ++completed_tasks_;
    if (waiting_) task_cv_.notify_one();
  }
}
