#pragma once

#include <array>
#include <cstddef>
#include <span>

#include "tauron/core/block.hpp"
#include "tauron/core/constants.hpp"

namespace tauron::core {

struct SequenceResult {
  std::array< DataBlock, SEQ_BLOCKS > blocks;
  bool eof;
  std::size_t count;
};

class Sequence {
public:
  static void build();
  static void parse();
};

}
