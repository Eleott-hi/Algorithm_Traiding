#include "matrix.h"


void Matrix::SwapRows(int row_1, int row_2) {
  for (int i = 0; i < cols_; i++)
    std::swap((*this)(row_1, i), (*this)(row_2, i));
}

std::ostream& operator<<(std::ostream& os, Matrix const& m) {
  for (int i = 0; i < m.Rows(); i++) {
    for (int j = 0; j < m.Cols(); j++) os << m(i, j) << "\t";
    os << "\n";
  }

  return os;
}

Matrix::Matrix(int rows, int cols) : rows_(rows), cols_(cols) { Init(); }

Matrix::~Matrix() { delete[] data_; }

Matrix::Matrix(const Matrix& other) : rows_(other.rows_), cols_(other.cols_) {
  Init();
  Copy(other);
}

Matrix::Matrix(Matrix&& other)
    : rows_(other.rows_), cols_(other.cols_), data_(other.data_) {
  other.cols_ = 0;
  other.rows_ = 0;
  other.data_ = nullptr;
}

Matrix& Matrix::operator=(const Matrix& other) {
  if (&other != this) {
    delete[] data_;
    rows_ = other.rows_;
    cols_ = other.cols_;
    Init();
    Copy(other);
  }

  return *this;
}

Matrix& Matrix::operator=(Matrix&& other) {
  if (&other != this) {
    delete[] data_;
    rows_ = other.rows_;
    cols_ = other.cols_;
    data_ = other.data_;
    other.rows_ = 0;
    other.cols_ = 0;
    other.data_ = nullptr;
  }

  return *this;
}

double Matrix::operator()(int row, int col) const {
  return data_[row * cols_ + col];
}

double& Matrix::operator()(int row, int col) {
  return data_[row * cols_ + col];
}

void Matrix::Init() {
  if (rows_ > 0 && cols_ > 0) data_ = new double[rows_ * cols_]{0};
}

void Matrix::Copy(const Matrix& other) {
  for (int i = 0; i < rows_ * cols_; ++i) data_[i] = other.data_[i];
}

