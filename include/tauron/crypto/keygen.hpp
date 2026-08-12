#pragma once

#include <array>
#include <cstdint>
#include <string_view>

#include "tauron/core/constants.hpp"
#include "tauron/utils/random.hpp"

namespace tauron::crypto {

using Key = std::array< std::uint8_t, core::KEY_SIZE >;

class KeyGen {
public:
  static Key deriveMaster( std::string_view passphrase, const utils::Salt& salt );
};

}
