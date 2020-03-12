#pragma once

#include <stdexcept>

namespace zeus::system
{
  [[using gnu: noreturn]] inline void throw_runtime_error(std::string target, std::string func, std::string reason)
  {
    std::string msg = target + "::" + func + ": " + reason;
    throw std::runtime_error{msg};
  }
}