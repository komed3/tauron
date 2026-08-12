#pragma once

#include <cstddef>
#include <cstdint>
#include <span>
#include <vector>

namespace tauron::utils {

class Permutation {
public:
  static std::vector< std::size_t > generate(
    const std::span< const std::uint8_t > context,
    const std::size_t size
  );
};

}
