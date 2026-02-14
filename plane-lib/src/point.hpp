#pragma once
#include "utils.hpp"

struct Point {
  const double x, y;

  Point(double x, double y) : x(x), y(y) {}

  bool equals(const Point &other) const {
    return double_eq(x, other.x) && double_eq(y, other.y);
  }
};
