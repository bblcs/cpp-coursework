#include <bit>
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

  friend Matrix operator-(Matrix lhs, const Matrix &rhs) {
    lhs -= rhs;
    return lhs;
  }

  Matrix &operator-=(const Matrix &other) {
    for (size_t i = 0; i < rows_ * cols_; ++i) {
      this->base_[i] -= other.base_[i];
    }
    return *this;
  }

  Matrix operator*(const Matrix &other) const { return smult(*this, other); }

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

private:
  friend Matrix cmult(const Matrix &lhs, const Matrix &rhs) {
    assert(lhs.cols_ == rhs.rows_);
    Matrix m(lhs.rows_, rhs.cols_);
    for (size_t i = 0; i < lhs.rows_; ++i) {
      for (size_t k = 0; k < lhs.cols_; ++k) {
        double aik = lhs[i][k];
        for (size_t j = 0; j < rhs.cols_; ++j) {
          m[i][j] += aik * rhs[k][j];
        }
      }
    }

    return m;
  }

  friend Matrix smult(const Matrix &lhs, const Matrix &rhs) {
    if (lhs.rows_ != lhs.cols_ || rhs.rows_ != rhs.cols_ ||
        lhs.rows_ == rhs.cols_ || std::popcount(lhs.rows_) != 1 ||
        lhs.rows_ <= 64) {
      return cmult(lhs, rhs);
    }

    auto half_n = lhs.rows_ / 2;

    Matrix a11 = lhs.submatrix(0, 0, half_n);
    Matrix a12 = lhs.submatrix(0, half_n, half_n);
    Matrix a21 = lhs.submatrix(half_n, 0, half_n);
    Matrix a22 = lhs.submatrix(half_n, half_n, half_n);
    Matrix b11 = rhs.submatrix(0, 0, half_n);
    Matrix b12 = rhs.submatrix(0, half_n, half_n);
    Matrix b21 = rhs.submatrix(half_n, 0, half_n);
    Matrix b22 = rhs.submatrix(half_n, half_n, half_n);

    Matrix m1 = smult(a11 + a22, b11 + b22);
    Matrix m2 = smult(a21 + a22, b11);
    Matrix m3 = smult(a11, b12 - b22);
    Matrix m4 = smult(a22, b21 - b11);
    Matrix m5 = smult(a11 + a12, b22);
    Matrix m6 = smult(a21 - a11, b11 + b12);
    Matrix m7 = smult(a12 - a22, b21 + b22);

    Matrix c11 = m1 + m4 - m5 + m7;
    Matrix c12 = m3 + m5;
    Matrix c21 = m2 + m4;
    Matrix c22 = m1 + m3 - m2 + m6;

    Matrix m(lhs.rows_);
    for (size_t i = 0; i < half_n; ++i) {
      for (size_t j = 0; j < half_n; ++j) {
        m[i][j] = c11[i][j];
        m[i][j + half_n] = c12[i][j];
        m[i + half_n][j] = c21[i][j];
        m[i + half_n][j + half_n] = c22[i][j];
      }
    }

    return m;
  }

  Matrix submatrix(size_t y, size_t x, size_t size) const {
    Matrix sub(size, size);
    for (size_t i = 0; i < size; ++i) {
      for (size_t j = 0; j < size; ++j) {
        sub[i][j] = (*this)[y + i][x + j];
      }
    }
    return sub;
  }
};
