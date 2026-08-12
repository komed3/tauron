#include "tauron/utils/permutation.hpp"

#include <algorithm>

namespace tauron::utils {

namespace {

constexpr std::uint64_t MIX_CONSTANT_A = 0xbf58476d1ce4e5b9ULL;
constexpr std::uint64_t MIX_CONSTANT_B = 0x94d049bb133111ebULL;
constexpr std::uint64_t SEED_CONSTANT =  0x9e3779b97f4a7c15ULL;

std::uint64_t mix( std::uint64_t value ) noexcept {
  value ^= value >> 30;
  value *= MIX_CONSTANT_A;
  value ^= value >> 27;
  value *= MIX_CONSTANT_B;
  value ^= value >> 31;

  return value;
}

std::uint64_t seed( std::span< const std::uint8_t > context ) noexcept {
  std::uint64_t state = SEED_CONSTANT;

  for ( const auto byte : context ) {
    state ^= byte;
    state = mix( state );
    state += SEED_CONSTANT;
  }

  return mix( state ^ context.size() );
}

std::uint64_t next( std::uint64_t& state ) noexcept {
  state += SEED_CONSTANT;
  return mix( state );
}

} // namespace

std::vector< std::size_t > Permutation::generate( std::span< const std::uint8_t > context, std::size_t size ) {
  std::vector< std::size_t > result( size );

  for ( std::size_t i = 0; i < size; ++i )
    result[ i ] = i;

  auto state = seed( context );

  for ( std::size_t i = size; i > 1; --i )
    std::swap( result[ i - 1 ], result[ next( state ) % i ] );

  return result;
}

}
