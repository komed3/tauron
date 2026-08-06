#include "tauron/utils/hex.hpp"

#include <iomanip>
#include <sstream>

namespace tauron::utils {

std::string hex( const std::vector< std::uint8_t >& bytes ) {
  std::ostringstream stream;

  for ( const auto& byte : bytes )
    stream
      << std::hex
      << std::setw( 2 )
      << std::setfill( '0' )
      << static_cast< int >( byte )
      << ' ';
}

}
