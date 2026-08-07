#pragma once

#include "tauron/core/profile.hpp"
#include "tauron/modules/registry.hpp"
#include "tauron/pipeline/pipeline.hpp"

namespace tauron::pipeline {

class Builder {
public:
  static Pipeline build( core::Profile selected, modules::Registry& registry );
};

}
