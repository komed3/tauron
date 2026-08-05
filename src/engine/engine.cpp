#include <tauron/engine/engine.hpp>

#include <utility>

namespace tauron {
  void Engine::use( std::unique_ptr< Module > module ) {
    modules.push_back( std::move( module ) );
  }
}
