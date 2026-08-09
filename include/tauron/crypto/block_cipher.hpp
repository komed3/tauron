#pragma once

#include <array>
#include <cstddef>
#include <cstdint>

#include "tauron/crypto/key_schedule.hpp"

namespace tauron::crypto {

inline constexpr std::size_t BLOCK_SIZE = 32;

using Block = std::array< std::uint8_t, BLOCK_SIZE >;

class BlockCipher {
public:
  [[nodiscard]] static Block encrypt( const Block& block, const RoundKeys& keys );
  [[nodiscard]] static Block decrypt( const Block& block, const RoundKeys& keys );
};

}
