#include "tauron/core/block.hpp"

#include <stdexcept>

#include "tauron/constants.hpp"

namespace tauron::core {

DataBlock Block::build( const std::uint8_t id, const std::span< const std::uint8_t > payload ) {
  if ( payload.size() > BLOCK_PAYLOAD )
    throw std::invalid_argument( "Block payload exceeds maximum bytes" );

  if ( id == 0xFF && ! payload.empty() )
    throw std::invalid_argument( "Random block cannot contain payload" );

  DataBlock block {};
  block[ 0 ] = id;
  block[ 1 ] = static_cast< std::uint8_t >( payload.size() );
}

ParsedBlock Block::parse( DataBlock block, std::uint8_t sequenceSize ) {}

}
