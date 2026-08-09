#pragma once

#include <array>
#include <cstddef>
#include <cstdint>

namespace tauron::crypto {

inline constexpr std::size_t WORDS = 8;

using Words = std::array< std::uint32_t, WORDS >;

constexpr std::uint32_t toWord( const std::array< std::uint8_t, 32 >& bytes, std::size_t offset ) noexcept {
  return static_cast< std::uint32_t >( bytes[ offset ] )
    | ( static_cast< std::uint32_t >( bytes[ offset + 1 ] ) <<  8 )
    | ( static_cast< std::uint32_t >( bytes[ offset + 2 ] ) << 16 )
    | ( static_cast< std::uint32_t >( bytes[ offset + 3 ] ) << 24 );
}

void toWords( const std::array< std::uint8_t, 32 >& bytes, Words& words ) noexcept {
  for ( std::size_t i = 0; i < words.size(); ++i )
    words[ i ] = toWord( bytes, i * 4 );
}

constexpr void fromWord( std::uint32_t word, std::array< std::uint8_t, 32 >& bytes, std::size_t offset ) noexcept {
  bytes[ offset ] = static_cast< std::uint8_t >( word );
  bytes[ offset + 1 ] = static_cast< std::uint8_t >( word >>  8 );
  bytes[ offset + 2 ] = static_cast< std::uint8_t >( word >> 16 );
  bytes[ offset + 3 ] = static_cast< std::uint8_t >( word >> 24 );
}

void fromWords( const Words& words, std::array< std::uint8_t, 32 >& bytes ) noexcept {
  for ( std::size_t i = 0; i < words.size(); ++i )
    fromWord( words[ i ], bytes, i * 4 );
}

}
