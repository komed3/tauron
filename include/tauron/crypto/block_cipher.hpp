#pragma once

#include <array>
#include <cstddef>
#include <cstdint>

#include "tauron/crypto/constants.hpp"
#include "tauron/crypto/key_schedule.hpp"

namespace tauron::crypto {

using Block = std::array< std::uint8_t, BLOCK_SIZE >;

class BlockCipher {
public:
  [[nodiscard]] static Block encrypt( const Block& block, const RoundKeys& keys ) noexcept;
  [[nodiscard]] static Block decrypt( const Block& block, const RoundKeys& keys ) noexcept;
};

}
