#pragma once

#include <string_view>
#include <vector>

#include "tauron/core/config.hpp"
#include "tauron/core/context.hpp"
#include "tauron/modules/registry.hpp"
#include "tauron/pipeline/pipeline.hpp"

namespace tauron::core {

class Engine {
public:
  explicit Engine( Config config = {} );
  void initialize();
  std::vector< std::uint8_t > process( std::string_view input, Operation operation );
};

}
