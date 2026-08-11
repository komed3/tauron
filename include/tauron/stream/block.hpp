#pragma once

#include <cstdint>
#include <span>
#include <vector>

#include "tauron/crypto/block_cipher.hpp"

namespace tauron::stream {

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
  static crypto::Block build( std::uint8_t id, std::span< const std::uint8_t > payload );
  static ParsedBlock parse( crypto::Block block, std::uint8_t sequenceSize );
};

}

