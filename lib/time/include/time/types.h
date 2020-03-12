#pragma once

#include <limits>
#include <chrono>

namespace zeus::time
{
  using timestamp_t = std::chrono::nanoseconds;
  constexpr timestamp_t invalid_time = timestamp_t{std::numeric_limits<timestamp_t::rep>::max()};
}