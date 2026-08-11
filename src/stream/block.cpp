#include "tauron/stream/block.hpp"

#include <stdexcept>

#include "tauron/crypto/nonce_generator.hpp"
#include "tauron/stream/checksum.hpp"
#include "tauron/stream/constants.hpp"

namespace tauron::stream {

crypto::Block Block::build( const std::uint8_t id, const std::span< const std::uint8_t > payload ) {
  if ( payload.size() > PAYLOAD_SIZE )
    throw std::invalid_argument( "Block payload exceeds 28 bytes" );

  if ( id == 0xFF && ! payload.empty() )
    throw std::invalid_argument( "Random block cannot contain payload" );

  crypto::Block block {};
  block[ 0 ] = id;
  block[ 1 ] = static_cast< std::uint8_t >( payload.size() );

  for ( std::size_t i = 0; i < payload.size(); ++i )
    block[ 2 + i ] = payload[ i ];

  if ( payload.size() < PAYLOAD_SIZE ) {
    const auto padding = crypto::NonceGenerator::generate();

    for ( std::size_t i = payload.size(); i < PAYLOAD_SIZE; ++i )
      block[ 2 + i ] = padding[ i - payload.size() ];
  }

  const auto checksum = Checksum::calculate( std::span< const std::uint8_t >{ block }.subspan( 2, 28 ) );
  block[ crypto::BLOCK_SIZE - 2 ] = static_cast< std::uint8_t >( checksum >> 8 );
  block[ crypto::BLOCK_SIZE - 1 ] = static_cast< std::uint8_t >( checksum & 0xFF );

  return block;
}

ParsedBlock Block::parse( crypto::Block block, std::uint8_t sequenceSize ) {
  ParsedBlock result{
    .id = block[ 0 ],
    .payload = {},
    .flag = BlockFlag::PASSED
  };

  const auto id = block[ 0 ];
  const auto length = block[ 1 ];

  if ( sequenceSize < MIN_SEQ_BLOCKS ) {
    result.flag = BlockFlag::INVALID_ID;
    return result;
  }

  if ( id != 0xFF && id >= sequenceSize ) {
    result.flag = BlockFlag::INVALID_ID;
    return result;
  }

  if ( length > 28 ) {
    result.flag = BlockFlag::INVALID_LENGTH;
    return result;
  }
}

}
