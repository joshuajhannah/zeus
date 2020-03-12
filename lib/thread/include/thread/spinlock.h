#pragma once

#include <atomic>
#include <xmmintrin.h>

#include "system/utilities.h"

namespace zeus::thread
{
  class spinlock
  {
  public:
    spinlock() = default;

    void lock() noexcept
    {
      do
      {
        while (_flag.load(std::memory_order_acquire))
        {
          /* TODO(jhannah): How much does PAUSE hurt on Skylake? */
          _mm_pause();
        }
      } while (__unlikely(_flag.exchange(true, std::memory_order_acq_rel)));
    }

    bool try_lock() noexcept
    {
      if(!_flag.load(std::memory_order_acquire))
      {
        return !_flag.exchange(true, std::memory_order_acq_rel);
      }

      return false;
    }

    void unlock() noexcept
    {
      utility::zassert(_flag.load(std::memory_order_acquire), "Unlocking a spinlock you do not own.");
      _flag.store(false, std::memory_order_release);
    }

  private:
    std::atomic<bool> _flag{false};
  };
}