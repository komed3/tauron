#pragma once

#include <cstdint>
#include <vector>

namespace tauron::stream {

enum class BlockFlag {
  PASSED,
  INVALID_LENGTH,
  INVALID_CHECKSUM
};

struct ParsedBlock {
  std::uint8_t id;
  std::vector< std::uint8_t > payload;
  BlockFlag flag;
};

}

