#include <gtest/gtest.h>
#include "math/fixed.h"

using namespace zeus::math;

TEST(MATH_FIXED, tparams)
{
  {
    fixed<6> foo{0};
    EXPECT_EQ(foo.underlying(), 0);
  }

  {
    fixed<4, uint32_t> foo{10};
    EXPECT_EQ(foo.underlying(), 100000U);
  }

  {
    fixed<5, int32_t> foo{100};
    EXPECT_EQ(foo.underlying(), 10000000);
  }
}

TEST(MATH_FIXED, limits)
{
  auto max = fixed<8>::max();
  EXPECT_EQ(max.underlying(), std::numeric_limits<int64_t>::max());

  auto min = fixed<8>::min();
  EXPECT_EQ(min.underlying(), std::numeric_limits<int64_t>::min());
}

TEST(MATH_FIXED, plus)
{
  {
    fixed<8> foo{10};
    fixed<8> bar{20};
    EXPECT_EQ(foo + bar, fixed<8>{30});
  }

  {
    fixed<8> foo{10};
    fixed<8> bar{-10};
    EXPECT_EQ(foo + bar, fixed<8>{0});
  }

  {
    fixed<8> foo{0};
    fixed<8> bar{0};
    EXPECT_EQ(foo + bar, fixed<8>{0});
  }

  {
    fixed<8> foo{-10};
    fixed<8> bar{-20};
    EXPECT_EQ(foo + bar, fixed<8>{-30});
  }

  {
    fixed<8> foo{-10};
    fixed<8> bar{0};
    EXPECT_EQ(foo + bar, fixed<8>{-10});
  }
}

TEST(MATH_FIXED, minus)
{
  {
    fixed<8> foo{10};
    fixed<8> bar{20};
    EXPECT_EQ(foo - bar, fixed<8>{-10});
  }

  {
    fixed<8> foo{10};
    fixed<8> bar{-10};
    EXPECT_EQ(foo - bar, fixed<8>{20});
  }

  {
    fixed<8> foo{0};
    fixed<8> bar{0};
    EXPECT_EQ(foo - bar, fixed<8>{0});
  }

  {
    fixed<8> foo{-10};
    fixed<8> bar{-20};
    EXPECT_EQ(foo - bar, fixed<8>{10});
  }

  {
    fixed<8> foo{-10};
    fixed<8> bar{0};
    EXPECT_EQ(foo - bar, fixed<8>{-10});
  }
}

TEST(MATH_FIXED, multiply)
{
  {
    fixed<8> foo{-10};
    EXPECT_EQ(foo * 1.0, foo);
  }

  {
    fixed<8> foo{-10};
    EXPECT_EQ(foo * 0.1, fixed<8>{-1});
  }
}

TEST(MATH_FIXED, divide)
{
  {
    fixed<8> foo{-10};
    EXPECT_EQ(foo / 1.0, foo);
  }

  {
    fixed<8> foo{-10};
    EXPECT_EQ(foo / 0.1, fixed<8>{-100});
  }
}

TEST(MATH_FIXED, equality)
{
  {
    fixed<8> foo{10};
    fixed<8> bar{5};
    EXPECT_EQ(foo, bar + bar);
    EXPECT_NE(foo, bar);
  }
}