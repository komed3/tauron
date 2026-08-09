#include "tauron/crypto/block_cipher.hpp"

#include <bit>

namespace tauron::crypto {

namespace {

constexpr std::uint32_t NONLINEAR_BASE = 0x9e3779b1;

std::uint32_t inverse32( std::uint32_t value ) noexcept {
  auto result = value;
  for ( int i = 0; i < 5; ++i ) result *= 2 - value * result;

  return result;
}

} // namespace

} // namespace tauron::crypto
