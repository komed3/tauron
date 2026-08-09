#pragma once

#include <array>
#include <cstddef>
#include <cstdint>
#include <vector>

namespace tauron::crypto {

using Key = std::array< std::uint8_t, 32 >;

class KeySchedule {
public:
  static std::vector< Key > expand( const Key& key, std::size_t rounds );
};

}
