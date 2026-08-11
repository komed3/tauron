#pragma once

#include <cstdint>
#include <vector>

namespace tauron::stream {

struct ParsedBlock {
  std::uint8_t id;
  std::vector< std::uint8_t > payload;
  bool valid;
};

}

