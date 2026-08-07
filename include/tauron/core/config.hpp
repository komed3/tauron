#pragma once

#include <cstdint>

namespace tauron::core {

enum class Profile {
  Default,
  Fast,
  Experimental
};

struct Config {
  Profile profile = Profile::Default;
  std::size_t blockSize = 64;
  bool debug = false;
};

}