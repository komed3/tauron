#include "tauron/pipeline/pipeline.hpp"

namespace tauron::pipeline {

void Pipeline::add( std::unique_ptr< modules::Module > module ) {
  modules.push_back( std::move( module ) );
}

}
