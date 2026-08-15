#pragma once

#include <array>
#include <cstddef>
#include <cstdint>
#include <span>
#include <vector>

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
  static SequenceResult build( std::span< const std::uint8_t > payload, bool eof );
  static std::size_t parse( std::span< const DataBlock > blocks, std::span< std::uint8_t > payload );
};

}
