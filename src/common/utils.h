#ifndef SRC_MAIN_MODEL_COMMON_UTILS_H_
#define SRC_MAIN_MODEL_COMMON_UTILS_H_

#include <algorithm>
#include <functional>
#include <random>
#include <vector>

namespace Utils {

double CalcStep(double num_1, double num_2);

template <typename T>
T Random(T from, T to) {
  std::random_device rd;
  std::uniform_int_distribution<T> uniform_dist(from, to);
  return uniform_dist(rd);
}

}  // namespace Utils

#endif  // SRC_MAIN_MODEL_COMMON_UTILS_H_
