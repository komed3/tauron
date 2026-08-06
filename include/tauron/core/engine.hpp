#pragma once

#include "tauron/core/context.hpp"
#include "tauron/pipeline/pipeline.hpp"

namespace tauron::core {

class Engine {
public:
  void run();

private:
  Context context;
  pipeline::Pipeline pipeline;
};

}
