#include "tauron/crypto/cipher.hpp"

namespace tauron::crypto {

namespace {

using Pair = std::array< std::size_t, 2 >;

constexpr std::array< Pair, 4 > PAIRS = {{ { 0, 1 }, { 2, 3 }, { 4, 5 }, { 6, 7 } }};
constexpr std::array< Pair, 4 > CROSS = {{ { 0, 2 }, { 1, 3 }, { 4, 6 }, { 5, 7 } }};

constexpr std::uint32_t NONLINEAR_BASE = 0x9e3779b1;

constexpr std::uint32_t inverse32( std::uint32_t value ) noexcept {
  auto result = value;
  for ( int i = 0; i < 5; ++i ) result *= 2 - value * result;

  return result;
}

} // namespace

core::Block Cipher::encrypt( const core::Block& block, const RoundKeys& keys ) noexcept {}

core::Block Cipher::decrypt( const core::Block& block, const RoundKeys& keys ) noexcept {}

}
