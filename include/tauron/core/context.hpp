#pragma once

#include "tauron/state/state.hpp"

namespace tauron::core {

struct Context {
  state::State state;
  std::vector< std::uint8_t > input;
  std::vector< std::uint8_t > output;
};

}
