#include "spline.h"

#include <cmath>

#include "gauss.h"

namespace Interpolation {

Spline::Spline(std::vector<double> const &x, std::vector<double> const &y)
    : x_(x), y_(y) {
  if (x.size() != y.size() || x.size() < 2) return;

  coefs_ = CalcCoef();
}

Matrix Spline::CalcCoef() {
  const int k_coef_num = 4;
  int splines = x_.size() - 1;
  Matrix matrix(splines * k_coef_num, splines * k_coef_num + 1);

  // Si = Ai(x-X0i)^3 + Bi(x-X0i)^2 + Ci(x-X0i) + Di

  int row = 0;

  // Fill Gauss Matrix
  for (int i = 0; i < splines; i++, row++) {
    matrix(row, matrix.Cols() - (splines - i) - 1) = 1;
    matrix(row, matrix.Cols() - 1) = y_.at(i);
  }

  for (int i = 0; i < splines; i++, row++) {
    double sub = x_.at(i + 1) - x_.at(i);

    matrix(row, i) = pow(sub, 3);
    matrix(row, i + splines) = pow(sub, 2);
    matrix(row, i + splines * 2) = sub;
    matrix(row, i + splines * 3) = 1;

    matrix(row, matrix.Cols() - 1) = y_.at(i + 1);
  }

  for (int i = 0; i < splines; i++, row++) {
    double sub = x_.at(i + 1) - x_.at(i);

    matrix(row, i) = 6 * sub;
    matrix(row, i + splines) = 2;
    if (i == splines - 1) continue;
    matrix(row, i + splines + 1) = -2;
  }

  for (int i = 0; i < splines - 1; i++, row++) {
    double sub = x_.at(i + 1) - x_.at(i);

    matrix(row, i) = 3 * pow(sub, 2);
    matrix(row, i + splines) = 2 * sub;
    matrix(row, i + splines * 2) = 1;
    matrix(row, i + splines * 2 + 1) = -1;
  }

  matrix(row, splines) = 1;

  auto coef = Gauss::Solve(matrix);
  if (coef.empty()) return {};

  Matrix coefs(splines, k_coef_num);
  for (int j = 0, ind = 0; j < coefs.Cols(); j++)
    for (int i = 0; i < coefs.Rows(); i++)  //
      coefs(i, j) = coef.at(ind++);

  return coefs;
}

double Spline::GetValue(double x) const {
  // Si = Ai(x-X0i)^3 + Bi(x-X0i)^2 + Ci(x-X0i) + Di

  for (int i = 0; i < coefs_.Rows(); i++)
    if (x_[i] <= x && x <= x_[i + 1]) {
      double sub = x - x_[i];
      return coefs_(i, 0) * pow(sub, 3) +  //
             coefs_(i, 1) * pow(sub, 2) +  //
             coefs_(i, 2) * sub +          //
             coefs_(i, 3);
    }

  return 0;
}

}  // namespace s21::Interpolation
