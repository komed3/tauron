#include "tauron/pipeline/pipeline.hpp"

namespace tauron::pipeline {

void Pipeline::add( modules::ModuleType type, std::unique_ptr< modules::Module > module ) {
  switch ( type ) {
    case modules::ModuleType::Setup:
      setup.push_back( std::move( module ) );
      break;

    case modules::ModuleType::Processing:
      processing.push_back( std::move( module ) );
      break;

    case modules::ModuleType::Finalization:
      finalization.push_back( std::move( module ) );
      break;
  }
}

void Pipeline::run( core::Context& context ) {
  execute( setup, context );
  execute( processing, context );
  execute( finalization, context );
}

void Pipeline::execute( Pipeline::ModuleList& modules, core::Context& context ) {
  for ( auto& module : modules ) module->execute( context );
}

}
