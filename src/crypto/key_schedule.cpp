#include "tauron/crypto/key_schedule.hpp"

#include <bit>
#include <stdexcept>

#include "tauron/crypto/words.hpp"

namespace tauron::crypto {

namespace {

inline constexpr std::array< std::size_t, KEY_SIZE > PERMUTATION = {
   0, 13, 26,  7, 20,  1, 14, 27,  8, 21,  2, 15, 28,  9, 22,  3,
  16, 29, 10, 23,  4, 17, 30, 11, 24,  5, 18, 31, 12, 19,  6, 25
};

inline constexpr std::uint32_t NONLINEAR_CONSTANT_A = 0x85ebca6b;
inline constexpr std::uint32_t NONLINEAR_CONSTANT_B = 0xc2b2ae35;
inline constexpr std::uint32_t ROUND_CONSTANT =       0x9e3779b9;

constexpr std::uint8_t substitute( std::uint8_t value ) noexcept {
  return static_cast< std::uint8_t >( value * 197 + 23 );
}

void substituteBytes( const Key& key, Key& bytes ) noexcept {
  for ( std::size_t i = 0; i < KEY_SIZE; ++i )
    bytes[ i ] = substitute( key[ i ] );
}

void localMix( Words& words ) noexcept {
  for ( std::size_t i = 0; i < words.size(); i += 2 ) {
    words[ i ] += std::rotl( words[ i + 1 ], 5 );
    words[ i + 1 ] ^= std::rotl( words[ i ], 13 );
  }
}

void crossMix( Words& words ) noexcept {
  for ( std::size_t i = 0; i < words.size(); ++i ) {
    const auto other = words[ ( i + 3 ) & 7 ];
    const auto rotation = static_cast< unsigned >( ( i * 7 + 3 ) & 31 );

    words[ i ] ^= std::rotl( other, rotation );
  }
}

Key permut( const Key& bytes ) noexcept {
  Key result {};

  for ( std::size_t i = 0; i < KEY_SIZE; ++i )
    result[ i ] = bytes[ PERMUTATION[ i ] ];

  return result;
}

Key deriveConstant( const Key& bytes, std::size_t round ) noexcept {
  auto value = ROUND_CONSTANT ^ static_cast< std::uint32_t >( round );

  for ( std::size_t i = 0; i < KEY_SIZE; ++i ) {
    value ^= bytes[ i ];
    value *= NONLINEAR_CONSTANT_A;
    value = std::rotl( value, 13 );
  }

  Key result {};

  for ( std::size_t i = 0; i < KEY_SIZE; ++i ) {
    value ^= static_cast< std::uint32_t >( i + round );
    value *= NONLINEAR_CONSTANT_B;
    value = std::rotl( value, 7 );

    result[ i ] = static_cast< std::uint8_t >(
      value ^ ( value >> 8 ) ^ ( value >> 16 ) ^ ( value >> 24 )
    );
  }

  return result;
}

Key transform( const Key& key, std::size_t round ) noexcept {
  Key bytes {};
  substituteBytes( key, bytes );

  Words words {};
  toWords( bytes, words );

  localMix( words );
  crossMix( words );

  fromWords( words, bytes );

  auto result = permut( bytes );
  const auto constant = deriveConstant( result, round );

  for ( std::size_t i = 0; i < KEY_SIZE; ++i )
    result[ i ] ^= constant[ i ];

  return result;
}

} // namespace

RoundKeys KeySchedule::expand( const Key& key, std::size_t rounds ) {
  if ( rounds < MIN_ROUNDS || rounds > MAX_ROUNDS )
    throw std::invalid_argument( "Rounds must be between 2 and 128" );

  RoundKeys keys;
  keys.reserve( rounds + 1 );
  keys.push_back( key );

  auto current = key;

  for ( std::size_t round = 1; round <= rounds; ++round ) {
    current = transform( current, round );
    keys.push_back( current );
  }

  return keys;
}

} // namespace tauron::crypto
