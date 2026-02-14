#pragma once

#include <cmath>

inline bool double_eq(double a, double b, double eps = 1e-9) {
  return std::fabs(a - b) < eps;
}
