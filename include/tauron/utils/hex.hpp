#pragma once

#include <cstdint>
#include <iomanip>
#include <sstream>
#include <string>

namespace tauron::utils {

template< typename Container >
std::string hex( const Container& bytes, char separator = ' ', bool uppercase = false ) {
  std::ostringstream stream;

  if ( uppercase ) stream << std::uppercase;
  stream << std::hex << std::setfill( '0' );

  bool first = true;

  for ( const auto byte : bytes ) {
    if ( ! first && separator ) stream << separator;

    stream << std::setw( 2 ) << static_cast< unsigned >( byte );
    first = false;
  }

  return stream.str();
};

}
