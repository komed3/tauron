#include "tauron/utils/permutation.hpp"

#include <algorithm>
#include <limits>

namespace tauron::utils {

namespace {

inline constexpr std::uint64_t MIX_CONSTANT_A = 0xbf58476d1ce4e5b9ULL;
inline constexpr std::uint64_t MIX_CONSTANT_B = 0x94d049bb133111ebULL;
inline constexpr std::uint64_t SEED_CONSTANT =  0x9e3779b97f4a7c15ULL;

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

std::uint64_t next( std::uint64_t state ) noexcept {
  state += SEED_CONSTANT;
  return mix( state );
}

} // namespace

}
