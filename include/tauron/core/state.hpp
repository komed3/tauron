#pragma once

#include <array>
#include <cstdint>

namespace tauron::core {

struct State {
  std::array< std::uint8_t, 32 > rootSeed {};
  std::array< std::uint64_t, 4 > registers {};
  std::uint64_t counter = 0;
  std::uint64_t epoch = 0;
};

}