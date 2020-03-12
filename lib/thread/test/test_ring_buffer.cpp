#include <gtest/gtest.h>
#include "thread/spsc_circular_buffer.h"

using namespace zeus::thread;

TEST(THREAD_RING_BUFFER, initialisation)
{
  spsc_circular_buffer<uint64_t, 4096> rb;
  rb.write(12);
  rb.write(13);

  {
    spsc_circular_buffer<uint64_t, 4096>::handle elem = rb.read();
    EXPECT_TRUE(static_cast<bool>(elem));
    EXPECT_EQ(static_cast<uint64_t>(elem), 12);
  }

  {
    spsc_circular_buffer<uint64_t, 4096>::handle elem = rb.read();
    EXPECT_TRUE(static_cast<bool>(elem));
    EXPECT_EQ(static_cast<uint64_t>(elem), 13);
  }

  {
    spsc_circular_buffer<uint64_t, 4096>::handle elem = rb.read();
    EXPECT_FALSE(static_cast<bool>(elem));
  }
}