#include "tauron/crypto/block_cipher.hpp"

#include <bit>

#include "tauron/crypto/words.hpp"

namespace tauron::crypto {

namespace {

constexpr std::uint32_t NONLINEAR_BASE = 0x9e3779b1;

constexpr std::uint32_t inverse32( std::uint32_t value ) noexcept {
  auto result = value;

  for ( int i = 0; i < 5; ++i )
    result *= 2 - value * result;

  return result;
}

constexpr unsigned injectionRotation( std::size_t round, std::size_t index ) noexcept {
  return ( round * 7 + index * 3 ) & 31;
}

constexpr unsigned diffusionRotationA( std::size_t round, std::size_t index ) noexcept {
  return ( round * 5 + index * 7 + 3 ) & 31;
}

constexpr unsigned diffusionRotationB( std::size_t round, std::size_t index ) noexcept {
  return ( round * 3 + index * 11 + 5 ) & 31;
}

constexpr unsigned butterflyRotationA( std::size_t round, std::size_t index ) noexcept {
  return ( round * 3 + index * 5 + 7 ) & 31;
}

constexpr unsigned butterflyRotationB( std::size_t round, std::size_t index ) noexcept {
  return ( round * 7 + index * 3 + 11 ) & 31;
}

constexpr unsigned crossRotationA( std::size_t round, std::size_t index ) noexcept {
  return ( round * 11 + index * 3 + 5 ) & 31;
}

constexpr unsigned crossRotationB( std::size_t round, std::size_t index ) noexcept {
  return ( round * 13 + index * 7 + 9 ) & 31;
}

constexpr std::uint32_t multiplier( std::size_t round, std::size_t index ) noexcept {
  return NONLINEAR_BASE + round * 2 + index * 2;
}

} // namespace

} // namespace tauron::crypto
