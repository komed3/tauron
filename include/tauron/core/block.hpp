#pragma once

#include <array>
#include <cstdint>
#include <span>
#include <vector>

#include "tauron/constants.hpp"

namespace tauron::core {

using DataBlock = std::array< std::int8_t, BLOCK_SIZE >;

enum class BlockFlag {
  PASSED,
  INVALID_ID,
  INVALID_CHECKSUM,
  INVALID_LENGTH
};

struct ParsedBlock {
  std::uint8_t id;
  std::vector< std::uint8_t > payload;
  BlockFlag flag;
};

class Block {
public:
  static DataBlock build( const std::uint8_t id, const std::span< const std::uint8_t > payload );
  static ParsedBlock parse( DataBlock block, std::uint8_t sequenceSize );
};

}
