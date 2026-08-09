#pragma once

#include <array>
#include <cstddef>
#include <cstdint>
#include <vector>

#include "tauron/crypto/parameters.hpp"

namespace tauron::crypto {

using Key = std::array< std::uint8_t, KEY_SIZE >;
using RoundKeys = std::vector< Key >;

class KeySchedule {
public:
  [[nodiscard]] static RoundKeys expand( const Key& key, std::size_t rounds );
};

}
