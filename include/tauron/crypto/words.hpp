#pragma once

#include <array>
#include <cstddef>
#include <cstdint>

namespace tauron::crypto {

constexpr std::size_t WORDS = 8;

using Words = std::array< std::uint32_t, WORDS >;

constexpr std::uint32_t toWord( const std::uint8_t* bytes ) noexcept {
  return static_cast< std::uint32_t >( bytes[ 0 ] )
    | ( static_cast< std::uint32_t >( bytes[ 1 ] ) <<  8 )
    | ( static_cast< std::uint32_t >( bytes[ 2 ] ) << 16 )
    | ( static_cast< std::uint32_t >( bytes[ 3 ] ) << 24 );
}

constexpr void fromWord( std::uint32_t word, std::uint8_t* bytes ) noexcept {
  bytes[ 0 ] = static_cast< std::uint8_t >( word );
  bytes[ 1 ] = static_cast< std::uint8_t >( word >>  8 );
  bytes[ 2 ] = static_cast< std::uint8_t >( word >> 16 );
  bytes[ 3 ] = static_cast< std::uint8_t >( word >> 24 );
}

template< typename Bytes >
constexpr Words toWords( const Bytes& bytes ) noexcept {
  Words words {};

  for ( std::size_t i = 0; i < words.size(); ++i )
    words[ i ] = toWord( bytes.data() + i * 4 );

  return words;
}

template< typename Bytes >
constexpr void fromWords( const Words& words, Bytes& bytes ) noexcept {
  for ( std::size_t i = 0; i < words.size(); ++i )
    fromWord( words[ i ], bytes.data() + i * 4 );
}

}