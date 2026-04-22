#include "avl.hpp"
#include "gtest/gtest.h"
#include <cstdlib>
#include <gtest/gtest.h>
#include <optional>

TEST(AvlTest, LinearInsertion) {
  AVL<int> a;
  for (int i = 0; i < 90; i++) {
    ASSERT_TRUE(a.insert(90 - 1 - i));
  }
}

TEST(AvlTest, ReversedLinearInsertion) {
  AVL<int> a;
  for (int i = 0; i < 90; i++) {
    ASSERT_TRUE(a.insert(i));
  }
}

TEST(AvlTest, LinearReinsertion) {
  AVL<int> a;
  ASSERT_TRUE(a.insert(10));
  for (int i = 0; i < 90; i++) {
    ASSERT_FALSE(a.insert(10));
  }
}

TEST(AvlTest, LinearDeletion) {
  AVL<int> a;
  for (int i = 0; i < 90; i++) {
    ASSERT_TRUE(a.insert(i));
  }
  for (int i = 0; i < 90; i++) {
    ASSERT_TRUE(a.remove(i));
  }
  for (int i = 0; i < 90; i++) {
    ASSERT_FALSE(a.remove(i));
  }
}

TEST(AvlTest, ReversedLinearDeletion) {
  AVL<int> a;
  for (int i = 0; i < 90; i++) {
    ASSERT_TRUE(a.insert(i));
  }
  for (int i = 0; i < 90; i++) {
    ASSERT_TRUE(a.remove(90 - 1 - i));
  }
  for (int i = 0; i < 90; i++) {
    ASSERT_FALSE(a.remove(90 - 1 - i));
  }
}

TEST(AvlTest, LinearFind) {
  AVL<int> a;
  for (int i = 0; i < 90; i++) {
    ASSERT_TRUE(a.insert(i));
  }
  for (int i = 0; i < 90; i++) {
    ASSERT_EQ(*a.find(i), i);
  }
  for (int i = 0; i < 90; i++) {
    ASSERT_TRUE(a.remove(i));
  }
  for (int i = 0; i < 90; i++) {
    ASSERT_FALSE(a.find(i).has_value());
  }
}

TEST(AvlTest, MinMaxSingleElement) {
  AVL<int> a;
  a.insert(1);
  EXPECT_EQ(*a.min(), 1);
  EXPECT_EQ(*a.max(), 1);
}

TEST(AvlTest, MinMaxAfterOperations) {
  AVL<int> a;
  a.insert(5);
  a.insert(3);
  a.insert(7);
  a.insert(2);
  a.insert(4);
  EXPECT_EQ(*a.min(), 2);
  EXPECT_EQ(*a.max(), 7);
  a.remove(2);
  EXPECT_EQ(*a.min(), 3);
  a.remove(7);
  EXPECT_EQ(*a.max(), 5);
}

TEST(AvlTest, SelectOnEmpty) {
  AVL<int> a;
  EXPECT_FALSE(a.select(1).has_value());
  EXPECT_FALSE(a.select(2).has_value());
}

TEST(AvlTest, SelectSingle) {
  AVL<int> a;
  a.insert(1);
  EXPECT_EQ(*a.select(0), 1);
  EXPECT_FALSE(a.select(2).has_value());
}

TEST(AvlTest, RankOnEmpty) {
  AVL<int> a;
  EXPECT_EQ(a.rank(1), 0);
}

TEST(AvlTest, RankSingle) {
  AVL<int> a;
  a.insert(1);
  EXPECT_EQ(a.rank(0), 0);
  EXPECT_EQ(a.rank(1), 0);
  EXPECT_EQ(a.rank(2), 1);
}

TEST(AvlTest, RankMultiple) {
  AVL<int> a;
  std::vector<int> vals = {5, 3, 8, 1, 4, 7, 9};
  for (int v : vals) {
    a.insert(v);
  }
  EXPECT_EQ(a.rank(0), 0);
  EXPECT_EQ(a.rank(1), 0);
  EXPECT_EQ(a.rank(2), 1);
  EXPECT_EQ(a.rank(3), 1);
  EXPECT_EQ(a.rank(4), 2);
  EXPECT_EQ(a.rank(5), 3);
  EXPECT_EQ(a.rank(6), 4);
  EXPECT_EQ(a.rank(7), 4);
  EXPECT_EQ(a.rank(8), 5);
  EXPECT_EQ(a.rank(9), 6);
  EXPECT_EQ(a.rank(10), 7);
}

TEST(AvlTest, SetFuzz) {
  AVL<int> a;
  std::set<int> s;
  std::srand(std::time(0));

  for (int i = 0; i < 2048; ++i) {
    int op = rand() % 3;
    int val = rand() % 90;
    if (op == 0) {
      bool inserted = a.insert(val);
      EXPECT_EQ(inserted, s.insert(val).second);
    } else if (op == 1) {
      bool removed = a.remove(val);
      EXPECT_EQ(removed, s.erase(val) == 1);
    } else {
      auto found = a.find(val);
      auto it = s.find(val);
      if (it != s.end())
        EXPECT_EQ(*found, val);
      else
        EXPECT_FALSE(found.has_value());
    }
    EXPECT_TRUE(a.verify());
  }
}

TEST(AvlTest, CopyCtorLinear) {
  AVL<int> a;

  for (int i = 0; i < 90; i++) {
    ASSERT_TRUE(a.insert(90 - 1 - i));
  }

  AVL<int> b(a);

  for (int i = 0; i < 90; i++) {
    ASSERT_TRUE(a.remove(90 - 1 - i));
    ASSERT_TRUE(b.remove(90 - 1 - i));
  }
  for (int i = 0; i < 90; i++) {
    ASSERT_FALSE(a.remove(90 - 1 - i));
    ASSERT_FALSE(b.remove(90 - 1 - i));
  }
}

TEST(AvlTest, CopyAsstLinear) {
  AVL<int> a;

  for (int i = 0; i < 90; i++) {
    ASSERT_TRUE(a.insert(90 - 1 - i));
  }

  AVL<int> b = a;

  for (int i = 0; i < 90; i++) {
    ASSERT_TRUE(a.remove(90 - 1 - i));
    ASSERT_TRUE(b.remove(90 - 1 - i));
  }
  for (int i = 0; i < 90; i++) {
    ASSERT_FALSE(a.remove(90 - 1 - i));
    ASSERT_FALSE(b.remove(90 - 1 - i));
  }
}

TEST(AvlTest, MoveCtorLinear) {
  AVL<int> a;

  for (int i = 0; i < 90; i++) {
    ASSERT_TRUE(a.insert(90 - 1 - i));
  }

  AVL<int> b(std::move(a));

  for (int i = 0; i < 90; i++) {
    ASSERT_FALSE(a.remove(90 - 1 - i));
    ASSERT_TRUE(b.remove(90 - 1 - i));
  }
  for (int i = 0; i < 90; i++) {
    ASSERT_FALSE(a.remove(90 - 1 - i));
    ASSERT_FALSE(b.remove(90 - 1 - i));
  }
}

TEST(AvlTest, MoveAsstLinear) {
  AVL<int> a;

  for (int i = 0; i < 90; i++) {
    ASSERT_TRUE(a.insert(90 - 1 - i));
  }

  AVL<int> b = std::move(a);

  for (int i = 0; i < 90; i++) {
    ASSERT_FALSE(a.remove(90 - 1 - i));
    ASSERT_TRUE(b.remove(90 - 1 - i));
  }
  for (int i = 0; i < 90; i++) {
    ASSERT_FALSE(a.remove(90 - 1 - i));
    ASSERT_FALSE(b.remove(90 - 1 - i));
  }
}

TEST(AvlTest, IteratorLinear) {
  AVL<int> a;
  for (int i = 0; i < 90; i++) {
    ASSERT_TRUE(a.insert(i));
  }
  for (AVL<int>::iterator it = a.begin(); it != a.end(); ++it) {
    ASSERT_TRUE(a.remove(*it));
  }
  for (int i = 0; i < 90; i++) {
    ASSERT_FALSE(a.remove(i));
  }
  ASSERT_EQ(a.begin(), a.end());
  for (int i = 0; i < 90; i++) {
    ASSERT_TRUE(a.insert(i));
  }
  for (int elem : a) {
    ASSERT_TRUE(a.remove(elem));
  }
  for (int i = 0; i < 90; i++) {
    ASSERT_FALSE(a.remove(i));
  }
  ASSERT_EQ(a.begin(), a.end());
}
