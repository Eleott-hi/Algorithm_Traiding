#ifndef SRC_MAIN_MODEL_COMMON_TIMER_H_
#define SRC_MAIN_MODEL_COMMON_TIMER_H_

#include <chrono>

using namespace std::chrono;
using namespace std::chrono_literals;

class Timer {
 public:
  milliseconds Finish() {
    return duration_cast<milliseconds>(GetTime() - timestamp_);
  }

 private:
  using Timestamp = high_resolution_clock::time_point;

  Timestamp timestamp_ = GetTime();

  Timestamp GetTime() { return high_resolution_clock::now(); }
};

#endif  // SRC_MAIN_MODEL_COMMON_TIMER_H_
