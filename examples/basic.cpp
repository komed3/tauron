#include <iostream>

#include "tauron/core/engine.hpp"
#include "tauron/utils/hex.hpp"

int main() {
  tauron::core::Config config;
  config.profile = tauron::core::Profile::Default;
  config.debug = true;

  tauron::core::Engine engine( config );
  engine.initialize();

  auto encrypted = engine.process(
    "Hello",
    tauron::core::Operation::Encrypt
  );

  std::cout
    << "Encrypted: "
    << tauron::utils::hex( encrypted )
    << "\n";
}
