#include "tauron/modules/evolution.hpp"

namespace tauron::modules {

void EvolutionModule::execute( core::Context& context ) {
  context.state.counter++;
}

}