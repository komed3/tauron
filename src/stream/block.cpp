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

  crypto::Block block {};
  block[ 0 ] = id;
  block[ 1 ] = static_cast< std::uint8_t >( payload.size() );

  for ( std::size_t i = 0; i < payload.size(); ++i )
    block[ 2 + i ] = payload[ i ];

  if ( payload.size() < 28 ) {
    const auto padding = crypto::NonceGenerator::generate();

    for ( std::size_t i = payload.size(); i < 28; ++i )
      block[ 2 + i ] = padding[ i - payload.size() ];
  }

  const auto checksum = Checksum::calculate( std::span< const std::uint8_t >{ block }.subspan( 2, 28 ) );
  block[ 30 ] = static_cast< std::uint8_t >( checksum >> 8 );
  block[ 31 ] = static_cast< std::uint8_t >( checksum & 0xFF );

  return block;
}

}
