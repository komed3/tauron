#pragma once

#include <array>
#include <cstddef>
#include <cstdint>
#include <vector>

namespace tauron::crypto {

inline constexpr std::size_t KEY_SIZE = 32;

using Key = std::array< std::uint8_t, KEY_SIZE >;
using RoundKeys = std::vector< Key >;

class KeySchedule {
public:
  [[nodiscard]] static RoundKeys expand( const Key& key, std::size_t rounds );
};

}
