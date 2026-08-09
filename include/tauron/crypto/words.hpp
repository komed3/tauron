#pragma once

#include <array>
#include <cstddef>
#include <cstdint>

namespace tauron::crypto {

using Words = std::array< std::uint32_t, 8 >;

constexpr std::uint32_t toWord( const std::uint8_t* bytes ) noexcept {
  return static_cast< std::uint32_t >( bytes[ 0 ] )
    | ( static_cast< std::uint32_t >( bytes[ 1 ] ) <<  8 )
    | ( static_cast< std::uint32_t >( bytes[ 2 ] ) << 16 )
    | ( static_cast< std::uint32_t >( bytes[ 3 ] ) << 24 );
}

}