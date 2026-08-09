#pragma once

#include <array>
#include <cstddef>
#include <cstdint>

#include "tauron/crypto/constants.hpp"

namespace tauron::crypto {

using Words = std::array< std::uint32_t, WORDS >;
using Bytes = std::array< std::uint8_t, BLOCK_SIZE >;

inline constexpr std::uint32_t toWord( const Bytes& bytes, std::size_t offset ) noexcept {
  return static_cast< std::uint32_t >( bytes[ offset ] )
    | ( static_cast< std::uint32_t >( bytes[ offset + 1 ] ) <<  8 )
    | ( static_cast< std::uint32_t >( bytes[ offset + 2 ] ) << 16 )
    | ( static_cast< std::uint32_t >( bytes[ offset + 3 ] ) << 24 );
}

inline void toWords( const Bytes& bytes, Words& words ) noexcept {
  for ( std::size_t i = 0; i < words.size(); ++i )
    words[ i ] = toWord( bytes, i * 4 );
}

inline constexpr void fromWord( std::uint32_t word, Bytes& bytes, std::size_t offset ) noexcept {
  bytes[ offset ] = static_cast< std::uint8_t >( word );
  bytes[ offset + 1 ] = static_cast< std::uint8_t >( word >>  8 );
  bytes[ offset + 2 ] = static_cast< std::uint8_t >( word >> 16 );
  bytes[ offset + 3 ] = static_cast< std::uint8_t >( word >> 24 );
}

inline void fromWords( const Words& words, Bytes& bytes ) noexcept {
  for ( std::size_t i = 0; i < words.size(); ++i )
    fromWord( words[ i ], bytes, i * 4 );
}

}
