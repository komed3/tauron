#include "tauron/core/block.hpp"

namespace tauron::core {

DataBlock Block::build( const std::uint8_t id, const std::span< const std::uint8_t > payload ) {}

ParsedBlock Block::parse( DataBlock block, std::uint8_t sequenceSize ) {}

}
