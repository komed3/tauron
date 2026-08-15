#pragma once

#include <array>
#include <cstddef>
#include <span>

#include "tauron/core/block.hpp"
#include "tauron/core/constants.hpp"

namespace tauron::core {

class Sequence {
public:
  static void build();
  static void parse();
};

}
