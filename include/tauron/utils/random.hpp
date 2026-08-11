#pragma once

#include <cstddef>
#include <cstdint>
#include <span>

namespace tauron::utils {

class Random {
public:
  static std::span< std::int8_t > generate( std::size_t length );
};

}