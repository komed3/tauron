#pragma once

#include <array>
#include <cstdint>
#include <string_view>
#include <vector>

#include "tauron/core/constants.hpp"
#include "tauron/utils/random.hpp"
#include "tauron/utils/word.hpp"

namespace tauron::crypto {

using Key = std::array< std::uint8_t, core::KEY_SIZE >;
using RoundKeys = std::vector< utils::Words >;

class KeyGen {
public:
  static Key deriveMaster( std::string_view passphrase, const utils::Salt& salt );
};

}
