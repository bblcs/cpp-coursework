#include "line.hpp"
#include "vec.hpp"
#include <gtest/gtest.h>

TEST(PlaneLibTest, VPParallelAreParallel) {
  Line alpha(Vec(1, 2), {1, 2});
  Line beta(Vec(2, 4), {19, -28});

  EXPECT_TRUE(alpha.parallel(beta));
}

TEST(PlaneLibTest, VPParallelDoesNotIntersect) {
  Line alpha(Vec(1, 2), {1, 2});
  Line beta(Vec(2, 4), {19, -28});

  EXPECT_EQ(alpha.intersection(beta), nullptr);
}

TEST(PlaneLibTest, VPNonParallelIntersect) {
  Line alpha(Vec(1, 1), {1, 2});
  Line beta(Vec(2, 4), {19, -28});
  Point *intersection = alpha.intersection(beta);

  EXPECT_NE(intersection, nullptr);

  delete intersection;
}
TEST(PlaneLibTest, CFParallelAreParallel) {
  Line alpha(3, 19);
  Line beta(3, 98);

  EXPECT_TRUE(alpha.parallel(beta));
}

TEST(PlaneLibTest, CFParallelDoesNotIntersect) {
  Line alpha(3, 19);
  Line beta(3, 98);

  EXPECT_EQ(alpha.intersection(beta), nullptr);
}

TEST(PlaneLibTest, CFNonParallelIntersect) {
  Line alpha(1, 0);
  Line beta(2, 0);
  Point *intersection = alpha.intersection(beta);

  EXPECT_NE(intersection, nullptr);

  delete intersection;
}
