#include "tauron/crypto/block_cipher.hpp"

#include <bit>

#include "tauron/crypto/words.hpp"

namespace tauron::crypto {

namespace {

constexpr std::uint32_t NONLINEAR_BASE = 0x9e3779b1;

constexpr std::array< std::array< std::size_t, 2 >, 4 > PAIRS = {{
  {{ 0, 1 }}, {{ 2, 3 }}, {{ 4, 5 }}, {{ 6, 7 }}
}};

constexpr std::array< std::array< std::size_t, 2 >, 4 > CROSS = {{
  {{ 0, 2 }}, {{ 1, 3 }}, {{ 4, 6 }}, {{ 5, 7 }}
}};

constexpr std::uint32_t inverse32( std::uint32_t value ) noexcept {
  auto result = value;
  for ( int i = 0; i < 5; ++i ) result *= 2 - value * result;

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

void inject( Words& words, const Words& keys, std::size_t round ) noexcept {
  for ( std::size_t i = 0; i < words.size(); ++i ) {
    words[ i ] += keys[ i ];
    words[ i ] ^= std::rotl( keys[ i ], injectionRotation( round, i ) );
  }
}

void inverseInject( Words& words, const Words& keys, std::size_t round ) noexcept {
  for ( std::size_t i = 0; i < words.size(); ++i ) {
    words[ i ] ^= std::rotl( keys[ i ], injectionRotation( round, i ) );
    words[ i ] -= keys[ i ];
  }
}

void nonlinear( Words& words, std::size_t round ) noexcept {
  for ( std::size_t i = 0; i < words.size(); ++i )
    words[ i ] *= multiplier( round, i );
}

void inverseNonlinear( Words& words, std::size_t round ) noexcept {
  for ( std::size_t i = 0; i < words.size(); ++i )
    words[ i ] *= inverse32( multiplier( round, i ) );
}

void mixPair( Words& words, std::size_t a, std::size_t b, unsigned r1, unsigned r2 ) noexcept {
  words[ a ] ^= std::rotl( words[ b ], r1 );
  words[ b ] += std::rotl( words[ a ], r2 );
}

void inverseMixPair( Words& words, std::size_t a, std::size_t b, unsigned r1, unsigned r2 ) noexcept {
  words[ b ] -= std::rotl( words[ a ], r2 );
  words[ a ] ^= std::rotl( words[ b ], r1 );
}

void diffuse( Words& words, std::size_t round ) noexcept {
  for ( std::size_t i = 0; i < words.size(); ++i ) {
    const auto next = ( i + 1 ) & 7;

    words[ i ] ^= std::rotl( words[ next ], diffusionRotationA( round, i ) );
    words[ next ] += std::rotl( words[ i ], diffusionRotationB( round, i ) );
  }
}

void inverseDiffuse( Words& words, std::size_t round ) noexcept {
  for ( std::size_t i = words.size(); i-- > 0; ) {
    const auto next = ( i + 1 ) & 7;

    words[ next ] -= std::rotl( words[ i ], diffusionRotationB( round, i ) );
    words[ i ] ^= std::rotl( words[ next ], diffusionRotationA( round, i ) );
  }
}

void butterfly( Words& words, std::size_t round ) noexcept {
  for ( const auto& [ a, b ] : PAIRS )
    mixPair( words, a, b, butterflyRotationA( round, a ), butterflyRotationB( round, b ) );

  for ( const auto& [ a, b ] : CROSS )
    mixPair( words, a, b, crossRotationA( round, a ), crossRotationB( round, b ) );
}

void inverseButterfly( Words& words, std::size_t round ) noexcept {
  for ( std::size_t i = CROSS.size(); i-- > 0; ) {
    const auto [ a, b ] = CROSS[ i ];
    inverseMixPair( words, a, b, crossRotationA( round, a ), crossRotationB( round, b ) );
  }

  for ( std::size_t i = PAIRS.size(); i-- > 0; ) {
    const auto [ a, b ] = PAIRS[ i ];
    inverseMixPair( words, a, b, butterflyRotationA( round, a ), butterflyRotationB( round, b ) );
  }
}

Block transform( const Block& block, const Key& key, std::size_t round ) noexcept {
  Words words {};
  Words keys {};

  toWords( block, words );
  toWords( key, keys );

  inject( words, keys, round );
  diffuse( words, round );
  nonlinear( words, round );
  butterfly( words, round );
  nonlinear( words, round );

  Block result {};
  fromWords( words, result );

  return result;
}

Block inverseTransform( const Block& block, const Key& key, std::size_t round ) noexcept {
  Words words {};
  Words keys {};

  toWords( block, words );
  toWords( key, keys );

  inverseNonlinear( words, round );
  inverseButterfly( words, round );
  inverseNonlinear( words, round );
  inverseDiffuse( words, round );
  inverseInject( words, keys, round );

  Block result {};
  fromWords( words, result );

  return result;
}

} // namespace

Block BlockCipher::encrypt( const Block& block, const RoundKeys& keys ) noexcept {
  Block state = block;

  for ( std::size_t round = 1; round < keys.size(); ++round )
    state = transform( state, keys[ round ], round );

  return state;
}

Block BlockCipher::decrypt( const Block& block, const RoundKeys& keys ) noexcept {
  Block state = block;

  for ( std::size_t round = keys.size(); round-- > 1; )
    state = inverseTransform( state, keys[ round ], round );

  return state;
}

} // namespace tauron::crypto
