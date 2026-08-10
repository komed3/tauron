#pragma once

#include <array>
#include <cstddef>
#include <cstdint>
#include <vector>

#include "tauron/crypto/constants.hpp"
#include "tauron/crypto/nonce_generator.hpp"
#include "tauron/crypto/words.hpp"

namespace tauron::crypto {

using Key = std::array< std::uint8_t, KEY_SIZE >;
using RoundKeys = std::vector< Words >;

class KeySchedule {
public:
  [[nodiscard]] static RoundKeys expand( const Key& key, const Nonce& nonce, std::size_t rounds );
};

}
