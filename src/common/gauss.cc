#include "gauss.h"

std::vector<double> Gauss::Solve(Matrix& matrix) {
  for (int k = 0; k < matrix.Rows(); k++) {
    int index = k;
    double max = std::abs(matrix(k, k));

    for (int i = k + 1; i < matrix.Rows(); i++)
      if (std::abs(matrix(i, k)) > max) {
        max = std::abs(matrix(i, k));
        index = i;
      }

    if (max == 0) {
      return std::vector<double>();
    }

    matrix.SwapRows(k, index);

    for (int i = k; i < matrix.Rows(); i++) {
      double tmp = matrix(i, k);
      if (tmp == 0) continue;
      for (int j = 0; j < matrix.Cols(); j++) matrix(i, j) /= tmp;
      if (i == k) continue;
      for (int j = 0; j < matrix.Cols(); j++) matrix(i, j) -= matrix(k, j);
    }
  }

  std::vector<double> res(matrix.Rows(), 0);
  for (int i = matrix.Rows() - 1; i >= 0; i--) {
    res[i] = matrix(i, matrix.Cols() - 1);
    for (int j = 0; j < i; j++)
      matrix(j, matrix.Cols() - 1) -= matrix(j, i) * res[i];
  }

  return res;
}
