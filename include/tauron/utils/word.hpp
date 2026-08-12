#pragma once

#include <array>
#include <cstddef>
#include <cstdint>

#include "tauron/core/constants.hpp"

namespace tauron::utils {

using Words = std::array< std::uint32_t, core::WORDS >;
using Bytes = std::array< std::uint8_t, core::BLOCK_SIZE >;

}
