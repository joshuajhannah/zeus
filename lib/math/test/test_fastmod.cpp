#include <gtest/gtest.h>
#include "math/fastmod.h"

using namespace zeus::math;

TEST(MATH_FASTMOD, granlund_divide)
{
  {
    granlund_fastmod foo{13};
    EXPECT_EQ(13 / foo, 1);
  }

  {
    granlund_fastmod foo{13};
    EXPECT_EQ(11 / foo, 0);
  }
  
  {
    granlund_fastmod foo{2};
    EXPECT_EQ(27 / foo, 13);
  }
}

TEST(MATH_FASTMOD, lemire_divide)
{
  {
    lemire_fastmod foo{13};
    EXPECT_EQ(13 / foo, 1);
  }

  {
    lemire_fastmod foo{13};
    EXPECT_EQ(11 / foo, 0);
  }

  {
    lemire_fastmod foo{2};
    EXPECT_EQ(27 / foo, 13);
  }
}

TEST(MATH_FASTMOD, granlund_modulo)
{
  {
    granlund_fastmod foo{13};
    EXPECT_EQ(13 % foo, 0);
  }

  {
    granlund_fastmod foo{13};
    EXPECT_EQ(11 % foo, 11);
  }

  {
    granlund_fastmod foo{2};
    EXPECT_EQ(27 % foo, 1);
  }
}

TEST(MATH_FASTMOD, lemire_modulo)
{
  {
    lemire_fastmod foo{13};
    EXPECT_EQ(13 % foo, 0);
  }

  {
    lemire_fastmod foo{13};
    EXPECT_EQ(11 % foo, 11);
  }

  {
    lemire_fastmod foo{2};
    EXPECT_EQ(27 % foo, 1);
  }
}