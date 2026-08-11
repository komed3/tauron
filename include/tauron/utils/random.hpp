#pragma once

#include <array>
#include <cstddef>
#include <cstdint>
#include <span>

#include "tauron/constants.hpp"

namespace tauron::utils {

using Nonce = std::array< std::uint8_t, NONCE_SIZE >;
using Salt = std::array< std::uint8_t, SALT_SIZE >;

class Random {
public:
  static std::span< std::int8_t > generate( std::size_t length );
  static Nonce nonce();
  static Salt salt();
};

}