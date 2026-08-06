#include "tauron/pipeline/pipeline.hpp"

namespace tauron::pipeline {

void Pipeline::registerModule( std::unique_ptr< modules::Module > module ) {
  modules.push_back( std::move( module ) );
}

void Pipeline::process( core::Context& context ) {
  for ( auto& module : modules ) module->process( context );
}

}
