#include <tauron/engine/engine.hpp>

#include <utility>

namespace tauron {
  void Engine::process( Context& context ) {
    for ( auto& module : modules ) {
      module->process( context );
    }
  }
}
