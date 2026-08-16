#include "tauron/core/sequence.hpp"

#include <algorithm>
#include <stdexcept>
#include <utility>

#include "tauron/utils/permutation.hpp"
#include "tauron/utils/random.hpp"

namespace tauron::core {

SequenceResult Sequence::build( std::span< const std::uint8_t > payload, bool eof ) {
  if ( payload.size() > SEQ_BLOCKS * BLOCK_PAYLOAD )
    throw std::invalid_argument( "Payload exceeds maximum sequence size" );

  SequenceResult result {};
  result.eof = eof;

  const auto count = eof ? ( payload.size() + BLOCK_PAYLOAD - 1 ) / BLOCK_PAYLOAD : SEQ_BLOCKS;

  std::array< std::uint8_t, BLOCK_SIZE > context {};
  context[ 0 ] = SEQ_PERMUT_DOMAIN;
  utils::Random::fill( std::span( context ).subspan( 1 ) );

  const auto permutation = utils::Permutation::generate( context, count );

  for ( std::size_t i = 0; i < count; ++i ) {
    const auto offset = i * BLOCK_PAYLOAD;
    const auto length = offset < payload.size() ? std::min( payload.size() - offset, BLOCK_PAYLOAD ) : 0;

    result.blocks[ permutation[ i ] ] = Block::build(
      static_cast< std::uint8_t >( i ), payload.subspan( offset, length )
    );
  }

  result.count = count;
  return result;
}

std::size_t Sequence::parse( std::span< const DataBlock > blocks, std::span< std::uint8_t > payload ) {
  if ( blocks.empty() ) return 0;

  if ( blocks.size() > SEQ_BLOCKS )
    throw std::invalid_argument( "Sequence contains too many blocks" );

  if ( payload.size() < blocks.size() * BLOCK_PAYLOAD )
    throw std::invalid_argument( "Payload buffer is too small" );

  std::array< ParsedBlock, SEQ_BLOCKS > parsed {};
  std::array< bool, SEQ_BLOCKS > seen {};

  for ( const auto& block : blocks ) {
    auto result = Block::parse( block );

    if ( result.flag != BlockFlag::PASSED )
      throw std::invalid_argument( "Invalid block" );

    if ( result.id >= blocks.size() )
      throw std::invalid_argument( "Invalid block ID" );

    if ( seen[ result.id ] )
      throw std::invalid_argument( "Duplicate block ID" );

    seen[ result.id ] = true;
    parsed[ result.id ] = std::move( result );
  }

  std::size_t size = 0;

  for ( std::size_t i = 0; i < blocks.size(); ++i ) {
    if ( ! seen[ i ] )
      throw std::invalid_argument( "Missing block ID" );

    std::copy(
      parsed[ i ].payload.begin(), parsed[ i ].payload.end(),
      payload.begin() + size
    );

    size += parsed[ i ].payload.size();
  }

  return size;
}

}
