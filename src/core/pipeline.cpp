#include "tauron/core/pipeline.hpp"

namespace tauron::core {

void Pipeline::add( std::unique_ptr< modules::Module > module ) {
  modules.push_back( std::move( module ) );
}

void Pipeline::process( Context& context ) {
  for ( auto& module : modules ) module->execute( context );
}

}