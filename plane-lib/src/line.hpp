#pragma once

#include "point.hpp"
#include "vec.hpp"

struct Line {
  Line(const Vec &dir, const Point &st) : direction(dir), start(st) {}
  Line(double m, double c) : direction(1, m), start({0, c}) {}
  Line(const Point &a, const Point &b)
      : direction(Vec(b.x - a.x, b.y - a.y)), start(a) {}

  bool parallel(const Line &other) const {
    return direction.equals(other.direction);
  }

  bool equals(const Line &other) const {
    return parallel(other) && start.equals(other.start);
  }

  Point *intersection(const Line &other) const {
    if (parallel(other) || direction.null() || other.direction.null()) {
      return nullptr;
    }

    double r =
        (start.x * other.direction.y - start.y * other.direction.x +
         other.start.y * other.direction.x -
         other.start.x * other.direction.y) /
        (direction.y * other.direction.x - direction.x * other.direction.y);

    double s = (start.x - other.start.x + r * direction.x) / other.direction.x;
    return new Point(r, s);
  }

  Line perpendicular(const Point &p) {
    Vec perp_dir = Vec(-direction.y, direction.x);
    return Line(perp_dir, p);
  }

private:
  const Vec direction;
  const Point start;
};
