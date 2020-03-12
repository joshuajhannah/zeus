#pragma once

#include <cstdint>

namespace zeus::math
{
  constexpr int64_t pow(int64_t value, uint64_t pow)
  {
    int64_t res = 1;

    while (pow-- > 0)
    {
      res *= value;
    }

    return res;
  }

  constexpr int64_t signof(int64_t value)
  {
    /* Return the sign bit */
    return static_cast<uint64_t>(value) >> (sizeof(int64_t) * 8 - 1);
  }
}