#include "tauron/utils/permutation.hpp"

#include <algorithm>
#include <limits>

namespace tauron::utils {

namespace {

inline constexpr std::uint64_t MIX_CONSTANT_A = 0xbf58476d1ce4e5b9ULL;
inline constexpr std::uint64_t MIX_CONSTANT_B = 0x94d049bb133111ebULL;

std::uint64_t mix( std::uint64_t value ) noexcept {
  value ^= value >> 30;
  value *= MIX_CONSTANT_A;
  value ^= value >> 27;
  value *= MIX_CONSTANT_B;
  value ^= value >> 31;

  return value;
}

} // namespace

}
