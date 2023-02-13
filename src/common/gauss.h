#ifndef A8_SRC_MAIN_MODEL_COMMON_GAUSS_H_
#define A8_SRC_MAIN_MODEL_COMMON_GAUSS_H_

#include <vector>

#include "matrix.h"

class Gauss {
 public:
  static std::vector<double> Solve(Matrix& matrix);
};

#endif  // A8_SRC_MAIN_MODEL_COMMON_GAUSS_H_
