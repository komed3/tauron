#pragma once

#include <array>
#include <cstdint>

namespace tauron::state {

struct State {
  std::array< std::uint8_t, 32 > seed {};
  std::uint64_t counter = 0;
};

}
