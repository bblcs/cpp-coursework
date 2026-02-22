#include "line.hpp"
#include "vec.hpp"
#include <gtest/gtest.h>
#include <optional>

TEST(PlaneLibTest, VPParallelAreParallel) {
  Line alpha(Vec(1, 2), {1, 2});
  Line beta(Vec(2, 4), {19, -28});

  EXPECT_TRUE(alpha.parallel(beta));
}

TEST(PlaneLibTest, VPParallelDoesNotIntersect) {
  Line alpha(Vec(1, 2), {1, 2});
  Line beta(Vec(2, 4), {19, -28});
  auto intersection = alpha.intersection(beta);
  EXPECT_FALSE(intersection.has_value());
}

TEST(PlaneLibTest, VPNonParallelIntersect) {
  Line alpha(Vec(1, 1), {1, 2});
  Line beta(Vec(2, 4), {19, -28});
  auto intersection = alpha.intersection(beta);
  EXPECT_TRUE(intersection.has_value());
}

TEST(PlaneLibTest, CFParallelAreParallel) {
  Line alpha(3, 19);
  Line beta(3, 98);

  EXPECT_TRUE(alpha.parallel(beta));
}

TEST(PlaneLibTest, CFParallelDoesNotIntersect) {
  Line alpha(3, 19);
  Line beta(3, 98);
  auto intersection = alpha.intersection(beta);
  EXPECT_FALSE(intersection.has_value());
}

TEST(PlaneLibTest, CFNonParallelIntersect) {
  Line alpha(1, 0);
  Line beta(2, 0);
  auto intersection = alpha.intersection(beta);
  EXPECT_TRUE(intersection.has_value());
}
