#include "tauron/crypto/block_cipher.hpp"

#include <bit>

namespace tauron::crypto {

namespace {

constexpr std::size_t WORDS = 8;
using Words = std::array< std::uint32_t, WORDS >;

} // namespace

} // namespace tauron::crypto
