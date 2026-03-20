#include <cassert>
#include <cstddef>
#include <cstdlib>
#include <vector>

class Matrix {
  std::vector<double> base_;
  size_t rows_;
  size_t cols_;

public:
  Matrix(size_t h, size_t w) : base_(w * h, 0.0), rows_(h), cols_(w) {}

  Matrix(size_t a) : Matrix(a, a) {}

  Matrix(const std::vector<double> &v) : Matrix(v.size()) {
    for (size_t i = 0; i < v.size(); ++i) {
      (*this)[i][i] = v[i];
    }
  }

  double *operator[](size_t row) { return &base_[row * cols_]; }
  const double *operator[](size_t row) const { return &base_[row * cols_]; }

  explicit operator double() const {
    double sum = 0;
    for (auto e : base_) {
      sum += e;
    }

    return sum;
  }

  friend Matrix operator+(Matrix lhs, const Matrix &rhs) {
    lhs += rhs;
    return lhs;
  }

  Matrix &operator+=(const Matrix &other) {
    for (size_t i = 0; i < rows_ * cols_; ++i) {
      this->base_[i] += other.base_[i];
    }
    return *this;
  }

  Matrix operator*(const Matrix &other) const {
    assert(cols_ == other.rows_);
    Matrix m = Matrix(rows_, other.cols_);
    for (size_t i = 0; i < rows_; ++i) {
      for (size_t j = 0; j < other.cols_; ++j) {
        double sum = 0;
        for (size_t k = 0; k < cols_; ++k) {
          sum += (*this)[i][k] * other[k][j];
        }
        m[i][j] = sum;
      }
    }

    return m;
  }

  Matrix &operator*=(const Matrix &other) {
    assert(cols_ == other.rows_);
    Matrix t = (*this) * other;
    *this = std::move(t);
    return *this;
  }

  friend Matrix operator*(Matrix lhs, const double d) {
    lhs *= d;
    return lhs;
  }

  Matrix &operator*=(const double d) {
    for (auto &e : this->base_) {
      e *= d;
    }

    return *this;
  }

  bool operator==(const Matrix &other) const { return base_ == other.base_; }
  bool operator!=(const Matrix &other) const { return base_ != other.base_; }
};
