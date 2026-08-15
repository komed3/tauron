#include "tauron/core/sequence.hpp"

namespace tauron::core {

SequenceResult Sequence::build( std::span< const std::uint8_t > payload, bool eof ) {}

std::vector< std::uint8_t > Sequence::parse( std::span< const DataBlock > blocks ) {}

}
