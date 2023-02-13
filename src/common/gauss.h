#ifndef SRC_MAIN_MODEL_COMMON_GAUSS_H_
#define SRC_MAIN_MODEL_COMMON_GAUSS_H_

#include <vector>

#include "matrix.h"

class Gauss {
 public:
  static std::vector<double> Solve(Matrix& matrix);
};

#endif  // SRC_MAIN_MODEL_COMMON_GAUSS_H_
