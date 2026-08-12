#include "tauron/core/block.hpp"

#include <stdexcept>

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

  const auto checksum = utils::Checksum::calculate(
    std::span< const std::uint8_t >( block ).subspan( 4 )
  );

  block[ 2 ] = static_cast< std::uint8_t >( checksum >> 8 );
  block[ 3 ] = static_cast< std::uint8_t >( checksum & 0xFF );

  return block;
}

ParsedBlock Block::parse( DataBlock block, std::uint8_t sequenceSize ) noexcept {
  ParsedBlock result { .id = block[ 0 ], .payload = {}, .flag = BlockFlag::PASSED };
  const auto id = block[ 0 ];
  const auto length = block[ 1 ];

  if ( id != 0xFF && id >= sequenceSize ) {
    result.flag = BlockFlag::INVALID_ID;
    return result;
  }

  if ( length > BLOCK_PAYLOAD ) {
    result.flag = BlockFlag::INVALID_LENGTH;
    return result;
  }

  const auto expectedChecksum =
    static_cast< std::uint16_t >( block[ 2 ] ) << 8 |
    static_cast< std::uint16_t >( block[ 3 ] );

  const auto checksum = utils::Checksum::calculate(
    std::span< const std::uint8_t >( block ).subspan( 4 )
  );

  if ( checksum != expectedChecksum ) {
    result.flag = BlockFlag::INVALID_CHECKSUM;
    return result;
  }

  result.payload.assign( block.begin() + 4, block.begin() + 4 + length );

  return result;
}

}
