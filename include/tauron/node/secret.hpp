#pragma once

#include <array>
#include <cstdint>

namespace tauron::node {

struct Secret {
  std::array< std::uint8_t, 32 > value {};
};

}
