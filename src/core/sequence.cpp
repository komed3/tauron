#include "tauron/core/sequence.hpp"

namespace tauron::core {

SequenceResult Sequence::build( std::span< const std::uint8_t > payload, bool eof ) {
  SequenceResult result {};
  result.eof = eof;

  const auto count = eof
    ? ( payload.size() + BLOCK_PAYLOAD - 1 ) / BLOCK_PAYLOAD
    : SEQ_BLOCKS;

  return result;
}

std::size_t Sequence::parse( std::span< const DataBlock > blocks, std::span< std::uint8_t > payload ) {}

}
