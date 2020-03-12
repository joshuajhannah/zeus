#pragma once

#include <cstdint>

#include "math/fixed.h"

/**
 * This file contains all of the type aliases associated with the core library
 */

namespace zeus::core
{
  /** Wish it was simpler to make aliases strongly typed!
   *
   *  TODO(jhannah): Write a wrapper class to do basically the same thing.
   */
  using quantity_t = int64_t;
  constexpr quantity_t invalid_quantity = std::numeric_limits<quantity_t>::max();

  using price_t = math::fixed<8>;
  constexpr price_t invalid_price = price_t::max();

  using clordid_t = int64_t;
  constexpr clordid_t invalid_clordid = std::numeric_limits<clordid_t>::max();

  using ordid_t = uint64_t;
  constexpr ordid_t invalid_ordid = std::numeric_limits<ordid_t>::max();
}