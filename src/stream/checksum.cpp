#include "tauron/stream/checksum.hpp"

namespace tauron::stream {

std::uint16_t Checksum::calculate( std::span< const std::uint8_t > payload ) {
  std::uint16_t checksum = 0xFFFF;

  return checksum;
}

}
