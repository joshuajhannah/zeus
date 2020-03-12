#pragma once

#include <cstdint>
#include "system/exception.h"
#include "system/utilities.h"

namespace zeus::math
{
  class granlund_fastmod
  {
  public:
    [[using gnu: cold, noinline]] explicit granlund_fastmod(uint64_t denominator) :
      _denominator(denominator)
    {
      utility::zassert_ndebug(denominator != 0, "Attempting to use fastmod optimisation with denominator of 0.");

      if (__builtin_popcount(denominator) == 1)
      {
        _magic = 0;
        _shift = static_cast<uint8_t>(sizeof(uint64_t) * 8 - 2 - __builtin_clzll(_denominator)) & _shift_mask;
      }
      else
      {
        __uint128_t const scaled =
          static_cast<__uint128_t>(1) << (sizeof(__uint128_t) * 8 - 1 - __builtin_clzll(_denominator));
        auto const magic = static_cast<uint64_t>(scaled / _denominator);
        auto const rem = static_cast<uint64_t>(scaled - magic * static_cast<__uint128_t>(_denominator));

        _magic = 2 * magic + static_cast<uint64_t>(rem * 2 >= _denominator || rem * 2 < rem) + 1;
        _shift = static_cast<uint8_t>((sizeof(uint64_t) * 8 - 1 - __builtin_clzll(_denominator)) | _add_marker) &
                 _shift_mask;
      }
    }

    inline uint64_t denominator() const noexcept { return _denominator; }

    friend uint64_t operator/(uint64_t, granlund_fastmod const &);

    friend uint64_t operator%(uint64_t, granlund_fastmod const &);

  private:
    static constexpr uint64_t _shift_mask{0x3F};
    static constexpr uint64_t _add_marker{0x40};
    uint64_t _denominator;
    uint64_t _magic;
    uint64_t _shift;
  };

  [[using gnu: hot, always_inline]] inline uint64_t operator/(uint64_t numerator, granlund_fastmod const &divisor)
  {
    auto const mulhi = static_cast<uint64_t>(static_cast<__uint128_t>(numerator) * divisor._magic >> 64);
    return ((numerator - mulhi) / 2 + mulhi) >> divisor._shift;
  }

  [[using gnu: hot, always_inline]] inline uint64_t operator%(uint64_t numerator, granlund_fastmod const &divisor)
  {
    return numerator - (divisor._denominator * (numerator / divisor));
  }

  class lemire_fastmod
  {
  public:
    [[using gnu: cold, noinline]] explicit lemire_fastmod(uint64_t denominator)
      : _magic{static_cast<__uint128_t>(-1) / denominator + 1}, _denominator{denominator}
    {
      utility::zassert_ndebug(denominator != 0, "Attempting to use fastmod optimisation with denominator of 0.");
    }

    inline uint64_t denominator() const noexcept { return _denominator; }

    friend uint64_t operator/(uint64_t, lemire_fastmod const &);

    friend uint64_t operator%(uint64_t, lemire_fastmod const &);

  private:
    __uint128_t _magic;
    uint64_t _denominator;
  };

  [[using gnu: hot, always_inline]] inline uint64_t operator/(uint64_t numerator, lemire_fastmod const& divisor)
  {
    return (((divisor._magic & ~0ULL) * numerator >> 64) + ((divisor._magic >> 64) * numerator)) >> 64;
  }

  [[using gnu: hot, always_inline]] inline uint64_t operator%(uint64_t numerator, lemire_fastmod const& divisor)
  {
    __uint128_t const magic = divisor._magic * numerator;
    return (((magic & ~0ULL) * divisor._denominator >> 64) + ((magic >> 64) * divisor._denominator)) >> 64;
  }
}