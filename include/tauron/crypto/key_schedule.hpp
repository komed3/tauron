#pragma once

#include <array>
#include <cstddef>
#include <cstdint>
#include <vector>

namespace tauron::crypto {

inline constexpr std::size_t KEY_SIZE = 32;

inline constexpr std::size_t MIN_ROUNDS = 2;
inline constexpr std::size_t MAX_ROUNDS = 128;

using Key = std::array< std::uint8_t, KEY_SIZE >;
using RoundKeys = std::vector< Key >;

class KeySchedule {
public:
  [[nodiscard]] static RoundKeys expand( const Key& key, std::size_t rounds );
};

}
