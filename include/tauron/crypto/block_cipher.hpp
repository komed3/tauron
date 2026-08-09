#pragma once

#include <array>
#include <cstddef>
#include <cstdint>

namespace tauron::crypto {

inline constexpr std::size_t BLOCK_SIZE = 32;

using Block = std::array< std::uint8_t, BLOCK_SIZE >;

class BlockCipher {
public:
  //
};

}
