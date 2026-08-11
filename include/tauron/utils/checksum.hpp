#pragma once

#include <cstdint>
#include <span>

namespace tauron::utils {

class Checksum {
public:
  static std::uint16_t calculate( std::span< const std::uint8_t > payload );
};

}
