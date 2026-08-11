#include "tauron/core/block.hpp"

#include <stdexcept>

#include "tauron/constants.hpp"
#include "tauron/utils/checksum.hpp"
#include "tauron/utils/random.hpp"

namespace tauron::core {

DataBlock Block::build( const std::uint8_t id, const std::span< const std::uint8_t > payload ) {
  if ( payload.size() > BLOCK_PAYLOAD )
    throw std::invalid_argument( "Block payload exceeds maximum bytes" );

  if ( id == 0xFF && ! payload.empty() )
    throw std::invalid_argument( "Random block cannot contain payload" );

  DataBlock block {};
  block[ 0 ] = id;
  block[ 1 ] = static_cast< std::uint8_t >( payload.size() );

  std::copy( payload.begin(), payload.end(), block.begin() + 4 );

  if ( payload.size() < BLOCK_PAYLOAD )
    utils::Random::fill( std::span( block.begin() + 4 + payload.size(), block.end() ) );

  const auto checksum = utils::Checksum::calculate( std::span< const std::uint8_t >( block ).subspan( 4 ) );
  block[ 2 ] = static_cast< std::uint8_t >( checksum >> 8 );
  block[ 3 ] = static_cast< std::uint8_t >( checksum & 0xFF );

  return block;
}

ParsedBlock Block::parse( DataBlock block, std::uint8_t sequenceSize ) {}

}
