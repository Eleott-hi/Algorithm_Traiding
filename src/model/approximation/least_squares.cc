#include "least_squares.h"

#include <utility>

#include "gauss.h"

namespace Approximation {
LeastSquares::LeastSquares(const std::vector<double> &x,  //
                           const std::vector<double> &y,  //
                           const std::vector<double> &w,  //
                           size_t degree) {
  if (x.size() != y.size() || x.size() != w.size() || x.empty() || degree == 0)
    return;

  x_ = x, y_ = y, w_ = w;
  coefs_ = CalcCoef(degree);
}

double LeastSquares::GetValue(double x) const {
  double res = 0;
  for (size_t i = 0; i < coefs_.size(); i++) res += coefs_[i] * pow(x, i);
  return res;
};

std::vector<double> LeastSquares::CalcCoef(size_t degree) {
  Matrix matrix(degree + 1, degree + 2);
  for (int i = 0; i < matrix.Rows(); ++i)
    for (int j = 0; j < matrix.Rows(); ++j)  //
      matrix(i, j) = CalcSumX(i + j);

  for (int i = 0; i < matrix.Rows(); ++i)
    matrix(i, matrix.Cols() - 1) = CalcSumY(i);

  return Gauss::Solve(matrix);
}

double LeastSquares::CalcSumX(size_t power) const {
  double res = 0;
  for (size_t i = 0; i < x_.size(); i++) res += w_.at(i) * pow(x_.at(i), power);
  return res;
}

double LeastSquares::CalcSumY(size_t power) const {
  double res = 0;
  for (size_t i = 0; i < x_.size(); i++)
    res += w_.at(i) * y_.at(i) * pow(x_.at(i), power);
  return res;
}

std::vector<double> LeastSquares::FindReverseSlopeWeights() {
  if (coefs_.size() != 2) return {};
  std::vector<double> tmp_weights = w_;
  w_ = std::vector<double>(w_.size(), 1);

  double a = coefs_.back();
  double a_wanted = -a;
  double lr = 1 * 2;

  constexpr double kMin = 1e-8;
  constexpr int kEpochAmount = 1000000;

  for (size_t epoch = 0;                                       //
       epoch < kEpochAmount && std::abs(a - a_wanted) > kMin;  //
       ++epoch) {
    double b = 0, c = 0, d = 0, e = 0, f = 0;

    for (size_t i = 0; i < x_.size(); ++i) {
      b += w_.at(i) * x_.at(i) * y_.at(i);
      c += w_.at(i) * x_.at(i) * x_.at(i);
      d += w_.at(i) * x_.at(i);
      e += w_.at(i) * y_.at(i);
      f += w_.at(i);
    }

    double k = c * f - d * d;
    double m = b * f - d * e;

    auto derivate = [&](size_t i) -> double {
      return ((y_.at(i) * x_.at(i) * f + b - x_.at(i) * e - d * y_.at(i)) * k -
              (x_.at(i) * x_.at(i) * f + c - 2 * d * x_.at(i)) * m) /
             (k * k);
    };

    for (size_t i = 0; i < w_.size(); ++i)
      w_.at(i) -= lr * (a - a_wanted) * derivate(i);

    a = CalcCoef(1).back();
  }

  return std::exchange(w_, std::move(tmp_weights));
}

}