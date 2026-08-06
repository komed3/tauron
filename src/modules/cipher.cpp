#include "tauron/modules/cipher.hpp"

#include <iostream>

#include "tauron/core/context.hpp"

namespace tauron::modules {

std::string_view CipherModule::name() const {
  return "Cipher";
}

void CipherModule::process( core::Context& context ) {
  std::cout
    << "[Cipher] processing state "
    << context.state.counter
    << "\n";
}

}
