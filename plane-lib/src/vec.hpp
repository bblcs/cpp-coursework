#pragma once

#include "utils.hpp"
#include <cmath>

struct Vec {
  double x, y;

  Vec(double x, double y) : x(x), y(y) {};

  double length() const { return std::sqrt(x * x + y * y); }

  Vec normalize() const {
    double len = length();
    return Vec{x / len, y / len};
  }

  bool equals(const Vec &other) const {
    Vec other_norm = other.normalize();
    Vec this_norm = normalize();
    return double_eq(other_norm.x, this_norm.x) &&
           double_eq(other_norm.y, this_norm.y);
  }

  bool null() const { return x == 0 && y == 0; }
};
