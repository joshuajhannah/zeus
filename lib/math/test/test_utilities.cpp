#include <gtest/gtest.h>
#include "math/utilities.h"

using namespace zeus::math;

TEST(MATH_UTILITIES, test_pow)
{
  int64_t pow_10_2 = pow(10, 2);
  EXPECT_EQ(pow_10_2, 100);

  int64_t pow_7_0 = pow(7, 0);
  EXPECT_EQ(pow_7_0, 1);

  int64_t pow_0_12 = pow(0, 12);
  EXPECT_EQ(pow_0_12, 0);
}
