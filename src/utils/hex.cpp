#include "tauron/utils/hex.hpp"

#include <iomanip>
#include <sstream>

namespace tauron::utils {

std::string hex( const std::vector< std::uint8_t >& bytes, char sep, bool upper ) {
  std::ostringstream stream;

  if ( upper ) stream << std::uppercase;
  stream << std::hex << std::setfill( '0' );

  bool first = true;

  for ( auto byte : bytes ) {
    if ( ! first && sep ) stream << sep;

    stream << std::setw( 2 ) << static_cast< int >( byte );
    first = false;
  }

  return stream.str();
}

}
