#pragma once

#include <array>
#include <cstdint>

#include "tauron/constants.hpp"

namespace tauron::core {

using Block = std::array< std::int8_t, BLOCK_SIZE >;

}
