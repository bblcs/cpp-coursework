#include "matrix.hpp"
#include <cmath>
#include <gtest/gtest.h>
#include <vector>

TEST(MatrixTest, IndexAccessOperator) {
  Matrix m = Matrix(10, 5);
  for (int y = 0; y < 10; ++y) {
    for (int x = 0; x < 5; ++x) {
      m[y][x] = (y + x) * (y < x ? 1 : -1);
    }
  }

  for (int y = 0; y < 10; ++y) {
    for (int x = 0; x < 5; ++x) {
      EXPECT_EQ(m[y][x], (y + x) * (y < x ? 1 : -1));
    }
  }
}

TEST(MatrixTest, VecConstructor) {
  std::vector<double> v = {1, 2, 3, 4, 5, 6, 7, 8, 9, 10};
  Matrix m = Matrix(v);

  for (size_t y = 0; y < v.size(); ++y) {
    for (size_t x = 0; x < v.size(); ++x) {
      if (y == x) {
        EXPECT_EQ(m[y][y], v[y]);
      } else {
        EXPECT_EQ(m[y][x], 0);
      }
    }
  }
}

TEST(MatrixTest, ExplicitTypeConversionOperator) {
  Matrix m = Matrix(10);
  EXPECT_EQ(static_cast<double>(m), 0);
  m[4][2] = 10;
  EXPECT_EQ(static_cast<double>(m), 10);
  m[2][2] = 12;
  EXPECT_EQ(static_cast<double>(m), 22);
}

TEST(MatrixTest, Equality) {
  Matrix a = Matrix(10);
  Matrix b = Matrix(10);

  EXPECT_TRUE(a == b);
  a[4][2] = 2;
  b[2][4] = 2;
  EXPECT_FALSE(a == b);
  a[2][4] = 2;
  b[4][2] = 2;
  EXPECT_TRUE(a == b);
}

TEST(MatrixTest, Inequality) {
  Matrix a = Matrix(10);
  Matrix b = Matrix(10);

  EXPECT_FALSE(a != b);
  a[4][2] = 2;
  b[2][4] = 2;
  EXPECT_TRUE(a != b);
  a[2][4] = 2;
  b[4][2] = 2;
  EXPECT_FALSE(a != b);
}

TEST(MatrixTest, Addition) {
  Matrix a = Matrix(3);
  Matrix b = Matrix(3);
  a[2][0] = 2;
  b[0][2] = 3;
  Matrix c = a + b;
  EXPECT_EQ(c, a + b);
  EXPECT_EQ(c[2][0], 2);
  EXPECT_EQ(c[0][2], 3);
}

void print(Matrix m) {
  for (int y = 0; y < 3; ++y) {
    for (int x = 0; x < 3; ++x) {
      std::cout << m[y][x] << " ";
    }
    std::cout << std::endl;
  }
}

TEST(MatrixTest, SquareMultiplication) {
  std::vector<double> ve = {1, 1, 1};
  Matrix a = Matrix(3);
  Matrix b = Matrix(3);
  Matrix e = Matrix(ve);
  a[2][0] = 2;
  b[0][2] = 3;
  a *= b;
  EXPECT_EQ(a, a * e);
  EXPECT_EQ(a[2][2], 6);
}

TEST(MatrixTest, NonSquareMultiplication) {
  Matrix a = Matrix(1, 2);
  Matrix b = Matrix(2, 3);
  double c = 0;

  for (size_t y = 0; y < 1; ++y) {
    for (size_t x = 0; x < 2; ++x) {
      a[y][x] = c++;
    }
  }
  for (size_t y = 0; y < 2; ++y) {
    for (size_t x = 0; x < 3; ++x) {
      b[y][x] = c++;
    }
  }

  a *= b;
  EXPECT_EQ(a[0][0], 5);
  EXPECT_EQ(a[0][1], 6);
  EXPECT_EQ(a[0][2], 7);
}

TEST(MatrixTest, StressScalarMultiplication) {
  size_t h = 1500;
  size_t w = 2031;
  Matrix a = Matrix(h, w);
  double c = 0;
  for (size_t y = 0; y < h; ++y) {
    for (size_t x = 0; x < w; ++x) {
      a[y][x] = c++;
    }
  }
  Matrix b = a * 18.3;
  a *= 18.3;

  c = 0;
  for (size_t y = 0; y < h; ++y) {
    for (size_t x = 0; x < w; ++x) {
      EXPECT_EQ(b[y][x], a[y][x]);
      EXPECT_EQ(a[y][x], (c++) * 18.3);
    }
  }
}
