#pragma once

#include <vector>

#include "state.hpp"

namespace tauron::core {

struct Context {
  core::State state;
  std::vector< std::uint8_t > input;
  std::vector< std::uint8_t > output;
};

}
