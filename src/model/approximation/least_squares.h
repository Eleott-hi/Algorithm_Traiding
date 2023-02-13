#ifndef SRC_MODEL_APPROXIMATION_LEAST_SQUARES_H_
#define SRC_MODEL_APPROXIMATION_LEAST_SQUARES_H_

#include <cmath>
#include <vector>

#include "base_approximation.h"

namespace Approximation {

class LeastSquares : public BaseApproximation {
 public:
  LeastSquares(const std::vector<double>& x,  //
               const std::vector<double>& y,  //
               const std::vector<double>& w,  //
               size_t degree);
  ~LeastSquares() = default;
  LeastSquares(LeastSquares&&) = delete;
  LeastSquares(const LeastSquares&) = delete;
  LeastSquares& operator=(LeastSquares&&) = delete;
  LeastSquares& operator=(const LeastSquares&) = delete;

  double GetValue(double x) const override;
  std::vector<double> FindReverseSlopeWeights();

 private:
  std::vector<double> coefs_;
  std::vector<double> x_, y_, w_;

  std::vector<double> CalcCoef(size_t degree);
  double CalcSumX(size_t power) const;
  double CalcSumY(size_t power) const;
};

}  // namespace Approximation

#endif  // SRC_MODEL_APPROXIMATION_LEAST_SQUARES_H_
