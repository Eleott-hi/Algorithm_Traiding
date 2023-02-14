#ifndef SRC_MODEL_COMMON_MATRIX_H_
#define SRC_MODEL_COMMON_MATRIX_H_

#include <ostream>

class Matrix {
 public:
  Matrix() = default;
  Matrix(int rows, int cols);
  ~Matrix();
  Matrix(Matrix&&);
  Matrix(const Matrix&);
  Matrix& operator=(Matrix&&);
  Matrix& operator=(const Matrix&);

  int Rows() const { return rows_; }
  int Cols() const { return cols_; }

  double operator()(int row, int col) const;
  double& operator()(int row, int col);

  void SwapRows(int row_1, int row_2);

  friend std::ostream& operator<<(std::ostream& os, Matrix const& m);

 private:
  int rows_ = 0;
  int cols_ = 0;
  double* data_ = nullptr;

  void Init();
  void Copy(const Matrix& other);
};

#endif  // SRC_MODEL_COMMON_MATRIX_H_
