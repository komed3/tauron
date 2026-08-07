#pragma once

#include <vector>

#include "config.hpp"
#include "state.hpp"

namespace tauron::core {

struct Buffers {
  std::vector< std::uint8_t > input;
  std::vector< std::uint8_t > output;
};

struct Runtime {
  std::uint64_t processedBlocks = 0;
  std::uint64_t processedBytes = 0;
};

struct Context {
  Config config;
  State state;
  Buffers buffers;
  Runtime runtime;
};

}
