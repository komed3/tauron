#include <iostream>

#include "tauron/core/context.hpp"
#include "tauron/modules/evolution.hpp"

namespace tauron::modules {

std::string_view EvolutionModule::name() const {
  return "Evolution";
}

void EvolutionModule::process( core::Context& context ) {
  context.state.counter++;

  std::cout
    << "[Evolution] counter = "
    << context.state.counter
    << "\n";
}

}
