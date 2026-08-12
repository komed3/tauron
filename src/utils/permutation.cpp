#include "tauron/utils/permutation.hpp"

#include <algorithm>
#include <limits>

namespace tauron::utils {

namespace {

std::uint64_t mix( std::uint64_t value ) noexcept {
  value ^= value >> 30;
  value *= 0xbf58476d1ce4e5b9ULL;
  value ^= value >> 27;
  value *= 0x94d049bb133111ebULL;
  value ^= value >> 31;

  return value;
}

} // namespace

}
