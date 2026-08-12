#pragma once

#include "tauron/core/block.hpp"
#include "tauron/crypto/keygen.hpp"

namespace tauron::crypto {

class Cipher {
public:
  static core::DataBlock encrypt( const core::DataBlock& block, const RoundKeys& keys ) noexcept;
  static core::DataBlock decrypt( const core::DataBlock& block, const RoundKeys& keys ) noexcept;
};

}
