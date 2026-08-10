#include "tauron/crypto/block_cipher.hpp"

#include <bit>

#include "tauron/crypto/words.hpp"

namespace tauron::crypto {

namespace {

constexpr std::uint32_t NONLINEAR_BASE = 0x9e3779b1;

constexpr std::array< std::array< std::size_t, 2 >, 4 > PAIRS = {{
  {{ 0, 1 }}, {{ 2, 3 }}, {{ 4, 5 }}, {{ 6, 7 }}
}};

constexpr std::array< std::array< std::size_t, 2 >, 4 > CROSS = {{
  {{ 0, 2 }}, {{ 1, 3 }}, {{ 4, 6 }}, {{ 5, 7 }}
}};

} // namespace

} // namespace tauron::crypto
