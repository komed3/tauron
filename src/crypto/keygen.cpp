#include "tauron/crypto/keygen.hpp"

#include <sodium.h>
#include <stdexcept>

namespace tauron::crypto {

namespace {

inline constexpr std::array< std::size_t, core::KEY_SIZE > PERMUTATION = {
   0, 13, 26,  7, 20,  1, 14, 27,  8, 21,  2, 15, 28,  9, 22,  3,
  16, 29, 10, 23,  4, 17, 30, 11, 24,  5, 18, 31, 12, 19,  6, 25
};

inline constexpr std::uint32_t NONLINEAR_CONSTANT_A = 0x85ebca6b;
inline constexpr std::uint32_t NONLINEAR_CONSTANT_B = 0xc2b2ae35;
inline constexpr std::uint32_t ROUND_CONSTANT =       0x9e3779b9;

constexpr std::uint8_t substitute( std::uint8_t value ) noexcept {
  return static_cast< std::uint8_t >( value * 197 + 23 );
}

void substituteBytes( const Key& key, const utils::Nonce& nonce, Key& bytes ) noexcept {
  for ( std::size_t i = 0; i < core::KEY_SIZE; ++i )
    bytes[ i ] = substitute( key[ i ] ^ nonce[ i ] );
}

void localMix( utils::Words& words ) noexcept {
  for ( std::size_t i = 0; i < words.size(); i += 2 ) {
    words[ i ] += std::rotl( words[ i + 1 ], 5 );
    words[ i + 1 ] ^= std::rotl( words[ i ], 13 );
  }
}

void crossMix( utils::Words& words ) noexcept {
  for ( std::size_t i = 0; i < words.size(); ++i ) {
    const auto other = words[ ( i + 3 ) & 7 ];
    const auto rotation = static_cast< unsigned >( ( i * 7 + 3 ) & 31 );

    words[ i ] ^= std::rotl( other, rotation );
  }
}

Key permut( const Key& bytes ) noexcept {
  Key result {};

  for ( std::size_t i = 0; i < core::KEY_SIZE; ++i )
    result[ i ] = bytes[ PERMUTATION[ i ] ];

  return result;
}

Key deriveConstant( const Key& bytes, std::size_t round ) noexcept {
  auto value = ROUND_CONSTANT ^static_cast< std::uint32_t >( round );

  for ( std::size_t i = 0; i < core::KEY_SIZE; ++i ) {
    value ^= bytes[ i ];
    value *= NONLINEAR_CONSTANT_A;
    value = std::rotl( value, 13 );
  }

  Key result {};

  for ( std::size_t i = 0; i < core::KEY_SIZE; ++i ) {
    value ^= static_cast< std::uint32_t >( i + round );
    value *= NONLINEAR_CONSTANT_B;
    value = std::rotl( value, 7 );

    result[ i ] = static_cast< std::uint8_t >(
      value ^ ( value >> 8 ) ^ ( value >> 16 ) ^ ( value << 24 )
    );
  }

  return result;
}

Key transform( const Key& key, const utils::Nonce& nonce, std::size_t round ) noexcept {
  Key bytes {};
  substituteBytes( key, nonce, bytes );

  utils::Words words {};
  utils::toWords( bytes, words );

  localMix( words );
  crossMix( words );

  utils::fromWords( words, bytes );

  auto result = permut( bytes );
  const auto constant = deriveConstant( result, round );

  for ( std::size_t i = 0; i < core::KEY_SIZE; ++i )
    result[ i ] ^= constant[ i ];

  return result;
}

} // namespace

Key KeyGen::derive( std::string_view passphrase, const utils::Salt& salt ) {
  Key key {};

  auto result = crypto_pwhash(
    key.data(), key.size(), passphrase.data(), passphrase.size(), salt.data(),
    crypto_pwhash_OPSLIMIT_MODERATE, crypto_pwhash_MEMLIMIT_MODERATE, crypto_pwhash_ALG_ARGON2ID13
  );

  if ( result != 0 )
    throw std::runtime_error( "Failed to derive master key" );

  return key;
}

RoundKeys KeyGen::expand( const Key& key, const utils::Nonce& nonce, std::size_t rounds ) {
  if ( rounds < core::MIN_ROUNDS || rounds > core::MAX_ROUNDS )
    throw std::invalid_argument( "Rounds must be between 2 and 128" );

  
}

}
