#ifndef SRC_MAIN_MODEL_COMMON_THREAD_POOL_H_
#define SRC_MAIN_MODEL_COMMON_THREAD_POOL_H_

#include <atomic>
#include <condition_variable>
#include <functional>
#include <mutex>
#include <queue>
#include <thread>
#include <unordered_set>

class ThreadPool {
 public:
  explicit ThreadPool(
      uint32_t thread_count = std::thread::hardware_concurrency());
  ~ThreadPool();
  ThreadPool(ThreadPool&&) = delete;
  ThreadPool(const ThreadPool&) = delete;
  ThreadPool& operator=(ThreadPool&&) = delete;
  ThreadPool& operator=(const ThreadPool&) = delete;

  void WaitAll();

  template <typename Task, typename... Args>
  size_t AddTask(Task const& task, Args... args) {
    auto task_func = std::bind(task, args...);

    std::scoped_lock lock(mtx_);
    size_t task_id = task_ids_++;
    queue_.push({task_func, task_id});
    task_available_cv_.notify_one();

    return task_id;
  }

 private:
  std::vector<std::thread> threads_;
  std::mutex mtx_;
  std::condition_variable task_available_cv_;
  std::queue<std::pair<std::function<void()>, size_t>> queue_;
  std::condition_variable task_cv_;
  std::atomic<size_t> task_ids_ = 0;
  std::atomic_bool running_ = false;
  std::atomic_bool waiting_ = false;
  std::atomic<uint32_t> thread_count_;
  std::atomic<size_t> completed_tasks_ = 0;
  std::unordered_set<size_t> completed_task_ids_;

  void Init();
  void Worker();
};

#endif  // SRC_MAIN_MODEL_COMMON_THREAD_POOL_H_
