#pragma once

#include <array>
#include <cstddef>
#include <cstdint>

#include "tauron/core/constants.hpp"

namespace tauron::utils {

using Words = std::array< std::uint32_t, core::WORDS >;
using Bytes = std::array< std::uint8_t, core::BLOCK_SIZE >;

inline constexpr std::uint32_t toWord( const Bytes& bytes, std::size_t offset ) noexcept {
  return
    ( static_cast< std::uint32_t >( bytes[ offset ] ) ) |
    ( static_cast< std::uint32_t >( bytes[ offset + 1 ] ) <<  8 ) |
    ( static_cast< std::uint32_t >( bytes[ offset + 2 ] ) << 16 ) |
    ( static_cast< std::uint32_t >( bytes[ offset + 3 ] ) << 24 );
}

inline void toWords( const Bytes& bytes, Words& words ) noexcept {
  for ( std::size_t i = 0; i < words.size(); ++i )
    words[ i ] = toWord( bytes, i * 4 );
}

}
