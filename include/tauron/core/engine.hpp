#pragma once

#include "context.hpp"
#include "tauron/pipeline/pipeline.hpp"

namespace tauron::core {

class Engine {
public:
  Engine();
  void run();

private:
  Context context;
  pipeline::Pipeline pipeline;
};

}
