#pragma once

#include "tauron/core/block.hpp"
#include "tauron/crypto/keygen.hpp"

namespace tauron::crypto {

class Cipher {
public:
  static core::Block encrypt( const core::Block& block, const RoundKeys& keys ) noexcept;
  static core::Block decrypt( const core::Block& block, const RoundKeys& keys ) noexcept;
};

}
