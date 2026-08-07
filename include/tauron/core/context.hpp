#pragma once

#include <vector>

#include "config.hpp"
#include "state.hpp"

namespace tauron::core {

enum class Operation {
  Encrypt,
  Decrypt
};

struct Buffers {
  std::vector< std::uint8_t > input;
  std::vector< std::uint8_t > output;
};

struct Context {
  Config config;
  Operation operation;
  State state;
  Buffers buffers;
};

}
