#include "tauron/pipeline/builder.hpp"
#include "tauron/pipeline/definition.hpp"

namespace tauron::pipeline {

Pipeline Builder::build( core::Profile selected, modules::Registry& registry ) {
  Pipeline pipeline;

  for ( const auto& step : profile( selected ) ) {
    auto factory = registry.find( std::string( step.module ) );
    if ( ! factory ) continue;

    for ( std::size_t i = 0; i < step.repeat; i++ )
      pipeline.add( step.type, ( *factory )() );
  }

  return pipeline;
}

}
