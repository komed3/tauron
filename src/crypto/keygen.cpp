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

}
