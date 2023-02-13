#include "utils.h"

namespace Utils {

double CalcStep(double num_1, double num_2) {
  return (num_2) ? num_1 / num_2 : num_2;
}

}  // namespace Utils
