#pragma once

#include <iostream>
#include <string>

#define __likely(x) __builtin_expect(!!(x), 1)
#define __unlikely(x) __builtin_expect(!!(x), 0)

namespace zeus::utility
{
  /**
   * TODO(jhannah): Use source_location when it becomes available in C++20
   */
  inline void zassert([[maybe_unused]] bool expr, [[maybe_unused]] std::string const& what)
  {
#ifndef NDEBUG
    if(__unlikely(!expr))
    {
      std::cout << "Assertion failed: " << what << std::endl;
      std::abort();
    }
#endif
  }

  /**
 * TODO(jhannah): Use source_location when it becomes available in C++20
 */
  inline void zassert_ndebug([[maybe_unused]] bool expr, [[maybe_unused]] std::string const& what)
  {
    if(__unlikely(!expr))
    {
      std::cout << "Assertion failed: " << what << std::endl;
      std::abort();
    }
  }
}