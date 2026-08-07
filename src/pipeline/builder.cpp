#include "tauron/pipeline/builder.hpp"
#include "tauron/pipeline/definition.hpp"

namespace tauron::pipeline {

Pipeline build( core::Profile selected, modules::Registry& registry ) {
  Pipeline pipeline;

  for ( const auto& step : profile( selected ) ) {}

  return pipeline;
}

}
