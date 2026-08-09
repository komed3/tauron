#include "tauron/crypto/key_schedule.hpp"

#include <bit>
#include <stdexcept>

namespace tauron::crypto {

namespace {

inline constexpr std::array< std::size_t, KEY_SIZE > PERMUTATION = {
   0, 13, 26,  7, 20,  1, 14, 27,  8, 21,  2, 15, 28,  9, 22,  3,
  16, 29, 10, 23,  4, 17, 30, 11, 24,  5, 18, 31, 12, 19,  6, 25
};

inline constexpr std::uint32_t NONLINEAR_CONSTANT_A = 0x85ebca6b;
inline constexpr std::uint32_t NONLINEAR_CONSTANT_B = 0xc2b2ae35;
inline constexpr std::uint32_t ROUND_CONSTANT =       0x9e3779b9;

using Words = std::array< std::uint32_t, 8 >;

constexpr std::uint32_t rotl( std::uint32_t value, unsigned bits ) noexcept {
  return std::rotl( value, bits );
}

constexpr std::uint8_t substitute( std::uint8_t value ) noexcept {
  return static_cast< std::uint8_t >( value * 197 + 23 );
}

std::uint32_t toWord( const Key& bytes, std::size_t offset ) noexcept {
  return static_cast< std::uint32_t >( bytes[ offset ] )
    | ( static_cast< std::uint32_t >( bytes[ offset + 1 ] ) << 8 )
    | ( static_cast< std::uint32_t >( bytes[ offset + 2 ] ) << 16 )
    | ( static_cast< std::uint32_t >( bytes[ offset + 3 ] ) << 24 );
}

void toWords( const Key& bytes, Words& words ) noexcept {
  for ( std::size_t i = 0; i < words.size(); ++i ) words[ i ] = toWord( bytes, i * 4 );
}

void substituteBytes( const Key& key, Key& bytes ) noexcept {
  for ( std::size_t i = 0; i < KEY_SIZE; ++i ) bytes[ i ] = substitute( key[ i ] );
}

}

}
