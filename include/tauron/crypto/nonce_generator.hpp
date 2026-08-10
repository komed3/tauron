#pragma once

#include <array>
#include <cstdint>

#include "tauron/crypto/constants.hpp"

namespace tauron::crypto {

using Nonce = std::array< std::uint8_t, NONCE_SIZE >;

class NonceGenerator {
public:
  static Nonce generate();
};

}
