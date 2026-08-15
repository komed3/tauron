#include "tauron/core/sequence.hpp"

#include "tauron/utils/permutation.hpp"
#include "tauron/utils/random.hpp"

namespace tauron::core {

SequenceResult Sequence::build( std::span< const std::uint8_t > payload, bool eof ) {
  SequenceResult result {};
  result.eof = eof;

  const auto count = eof
    ? ( payload.size() + BLOCK_PAYLOAD - 1 ) / BLOCK_PAYLOAD
    : SEQ_BLOCKS;

  std::array< std::uint8_t, BLOCK_SIZE > context {};
  utils::Random::fill( context );

  const auto permutation = utils::Permutation::generate( context, count );

  for ( std::size_t i = 0; i < count; ++i ) {
    const auto offset = i * BLOCK_PAYLOAD;
    const auto length = offset < payload.size() ? std::min( payload.size() - offset, BLOCK_PAYLOAD ) : 0;

    result.blocks[ i ] = Block::build(
      static_cast< std::uint8_t >( permutation[ i ] ),
      payload.subspan( offset, length )
    );
  }

  result.count = count;
  return result;
}

std::size_t Sequence::parse( std::span< const DataBlock > blocks, std::span< std::uint8_t > payload ) {}

}
