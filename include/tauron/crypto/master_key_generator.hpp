#pragma once

#include <array>
#include <cstdint>
#include <string_view>

#include "tauron/crypto/constants.hpp"

namespace tauron::crypto {

using MasterKey = std::array< std::uint8_t, KEY_SIZE >;
using Salt = std::array< std::uint8_t, SALT_SIZE >;

class MasterKeyGenerator {
public:
  static MasterKey generate( std::string_view passphrase, const Salt& salt );
};

}
