#ifndef A8_SRC_MAIN_MODEL_APPROXIMATION_SPLINE_H_
#define A8_SRC_MAIN_MODEL_APPROXIMATION_SPLINE_H_

#include <vector>

#include "base_approximation.h"
#include "matrix.h"

namespace Interpolation {

class Spline : public BaseApproximation {
 public:
  Spline(std::vector<double> const &x, std::vector<double> const &y);
  ~Spline() = default;
  Spline(Spline &&) = delete;
  Spline(const Spline &) = delete;
  Spline &operator=(Spline &&) = delete;
  Spline &operator=(const Spline &) = delete;

  virtual double GetValue(double x) const override;

 private:
  Matrix coefs_;
  const std::vector<double> &x_, &y_;

  Matrix CalcCoef();
};

}  // namespace Interpolation

#endif  // A8_SRC_MAIN_MODEL_APPROXIMATION_SPLINE_H_
