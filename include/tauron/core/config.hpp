#pragma once

#include <cstddef>

#include "tauron/core/profile.hpp"

namespace tauron::core {

struct Config {
  Profile profile = Profile::Default;
  std::size_t blockSize = 64;
  bool debug = false;
};

}