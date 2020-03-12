#pragma once

#include <atomic>
#include <filesystem>
#include <cstring>
#include <sys/mman.h>
#include <unistd.h>

#include "system/exception.h"
#include "system/utilities.h"

namespace zeus::thread
{
  template <typename T, std::size_t N>
  class spsc_circular_buffer
  {
    /* For performance reasons, this particular ring buffer does not allow non-pow-2 sizes */
    static_assert(__builtin_popcount(N) == 1);

  public:
    class handle
    {
    public:
      explicit constexpr operator T const&() const noexcept
      {
        return *reinterpret_cast<T*>(_rb._buffer + _rb._begin.load(std::memory_order_relaxed) % N);
      }

      explicit constexpr operator bool() const noexcept
      {
        return _rb._begin.load(std::memory_order_relaxed) - _end > 0;
      }

      ~handle()
      {
        if(static_cast<bool>(*this))
        {
          _rb._begin.fetch_add(sizeof(T), std::memory_order_relaxed);
        }
      }

    private:
      handle(spsc_circular_buffer &rb)
        : _rb{rb} {}

    private:
      spsc_circular_buffer& _rb;
      size_t _end{_rb._end.load(std::memory_order_acquire)};

    private:
      friend spsc_circular_buffer;
    };

  public:
    spsc_circular_buffer()
    {
      /* First mmap, used purely to reserve the virtual address space */
      _buffer = reinterpret_cast<std::byte*>(::mmap(nullptr, 2 * N, PROT_READ | PROT_WRITE, MAP_PRIVATE | MAP_ANONYMOUS, -1, 0));
      if(_buffer == nullptr)
      {
        zeus::system::throw_runtime_error("spsc_circular_buffer", __func__, "Failed to create initial mapping.");
      }

      /* TODO(jhannah): Enable option to use hugepages */
      /* Open a file used to capture some underlying memory */
      std::filesystem::path tmpname{std::tmpnam(nullptr)};
      int fd = memfd_create(tmpname.filename().string().data(), MFD_CLOEXEC);
      if(fd == -1)
      {
        std::string msg = std::string{"Failed to create file descriptor: "} + std::strerror(errno);
        zeus::system::throw_runtime_error("spsc_circular_buffer", __func__, std::move(msg));
      }

      /* Resize it to our needs */
      int truncated = ftruncate(fd, N);
      if(truncated != 0)
      {
        std::string msg = std::string{"Failed to truncate file: "} + std::strerror(truncated);
        zeus::system::throw_runtime_error("spsc_circular_buffer", __func__, std::move(msg));
      }

      /* Now create the two underlying, contiguous mappings */
      if(::mmap(_buffer, N,PROT_READ | PROT_WRITE, MAP_PRIVATE | MAP_POPULATE | MAP_FIXED, fd, 0) == nullptr ||
         ::mmap(_buffer + N, N, PROT_READ | PROT_WRITE, MAP_PRIVATE | MAP_POPULATE | MAP_FIXED, fd, 0) == nullptr)
      {
        std::string msg = std::string{"Failed to create magic mapping: "} + std::strerror(errno);
        zeus::system::throw_runtime_error("spsc_circular_buffer", __func__, std::move(msg));
      }

      /* We don't need the open file descriptor any more */
      int closed = close(fd);
      if(closed == -1)
      {
        std::string msg = std::string{"Failed to close file descriptor: "} + std::strerror(errno);
        zeus::system::throw_runtime_error("spsc_circular_buffer", __func__, std::move(msg));
      }
    }

    ~spsc_circular_buffer()
    {
      ::munmap(_buffer, N);
      ::munmap(_buffer + N, N);
    }

    handle read()
    {
      return handle{*this};
    }

    void write(T elem)
    {
      if (__likely(_end.load(std::memory_order_relaxed) - _begin.load(std::memory_order_relaxed) <= N - sizeof(T)))
      {
        std::memcpy(_buffer + _end.load(std::memory_order_relaxed) % N, std::addressof(elem), sizeof(T));
        _end.fetch_add(sizeof(T), std::memory_order_relaxed);
      }
    }

    void reset()
    {
      _begin.store(0, std::memory_order_release);
      _end.store(0, std::memory_order_release);
    }

    bool empty()
    {
      return _begin.load(std::memory_order_acquire) == _end.load(std::memory_order_relaxed);
    }

    std::size_t capacity()
    {
      return N / sizeof(T);
    }

    std::size_t size()
    {
      return (_end.load(std::memory_order_acquire) - _begin.load(std::memory_order_acquire)) / sizeof(T);
    }

  private:
    /* This has to be volatile. The most intuitive explanation is that the pointer to the beginning and the pointer to
     * one past the end actually alias the same memory, but this is invisible to the compiler. */
    std::byte* _buffer{nullptr};

    std::atomic<std::size_t> _begin{0};
    std::atomic<std::size_t> _end{0};
  };
}