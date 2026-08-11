#include "tauron/stream/checksum.hpp"

namespace tauron::stream {

std::uint16_t Checksum::calculate( std::span< const std::uint8_t > payload ) {
  std::uint16_t checksum = 0xFFFF;

  for ( const auto byte : payload ) {
    checksum ^= static_cast< std::uint16_t >( byte ) << 8;

    for ( int i = 0; i < 8; ++i )
      checksum = checksum & 0x8000
        ? static_cast< std::uint16_t >( ( checksum << 1 ) ^ 0x1021 )
        : static_cast< std::uint16_t >( checksum << 1 );
  }

  return checksum;
}

}
