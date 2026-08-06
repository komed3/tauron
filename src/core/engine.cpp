#include "tauron/core/engine.hpp"

#include <iostream>
#include <memory>

#include "tauron/modules/cipher.hpp"
#include "tauron/modules/evolution.hpp"

namespace tauron::core {

Engine::Engine() {
  pipeline.registerModule( std::make_unique< modules::EvolutionModule >() );
  pipeline.registerModule( std::make_unique< modules::CipherModule >() );
}

void Engine::run() {
  std::cout << "Starting Tauron ... \n";
  pipeline.process( context );
  std::cout << "Finished.\n";
}

}
