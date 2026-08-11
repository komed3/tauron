#include "tauron/stream/block.hpp"

#include <stdexcept>

#include "tauron/crypto/nonce_generator.hpp"
#include "tauron/stream/checksum.hpp"
#include "tauron/stream/constants.hpp"

namespace tauron::stream {

crypto::Block Block::build( const std::uint8_t id, const std::span< const std::uint8_t > payload ) {
  if ( payload.size() > 28 )
    throw std::invalid_argument( "Block payload exceeds 28 bytes" );

  if ( id == 0xFF && ! payload.empty() )
    throw std::invalid_argument( "Random block cannot contain payload" );
}

}
