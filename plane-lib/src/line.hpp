#pragma once

#include "point.hpp"
#include "vec.hpp"
#include <optional>

class Line {
  const Vec direction;
  const Point start;

public:
  static std::optional<Line> from_dir_and_point(const Vec &dir,
                                                const Point &st) {
    if (dir.length() == 0)
      return std::nullopt;

    return Line(dir, st);
  }

  static std::optional<Line> from_constants(double m, double c) {
    if (m == 0)
      return std::nullopt;

    return Line(m, c);
  }

  static std::optional<Line> from_points(const Point &a, const Point &b) {
    if (a.equals(b))
      return std::nullopt;

    return Line(a, b);
  }

  bool parallel(const Line &other) const {
    return direction.equals(other.direction);
  }

  bool equals(const Line &other) const {
    return parallel(other) && start.equals(other.start);
  }

  std::optional<Point> intersection(const Line &other) const {
    if (parallel(other) || direction.null() || other.direction.null()) {
      return std::nullopt;
    }

    double r =
        (start.x * other.direction.y - start.y * other.direction.x +
         other.start.y * other.direction.x -
         other.start.x * other.direction.y) /
        (direction.y * other.direction.x - direction.x * other.direction.y);

    double s = (start.x - other.start.x + r * direction.x) / other.direction.x;
    return Point(r, s);
  }

  Line perpendicular(const Point &p) {
    Vec perp_dir = Vec(-direction.y, direction.x);
    return Line(perp_dir, p);
  }

private:
  Line(const Vec &dir, const Point &st) : direction(dir), start(st) {}
  Line(double m, double c) : direction(1, m), start({0, c}) {}
  Line(const Point &a, const Point &b)
      : direction(Vec(b.x - a.x, b.y - a.y)), start(a) {}
};
