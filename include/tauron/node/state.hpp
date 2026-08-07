#pragma once

#include <cstdint>

namespace tauron::node {

struct State {
  std::uint64_t epoch = 0;
  std::uint64_t counter = 0;
};

}
