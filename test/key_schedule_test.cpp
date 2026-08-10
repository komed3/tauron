#include <iostream>

#include "tauron/crypto/key_schedule.hpp"
#include "tauron/crypto/nonce_generator.hpp"

using namespace tauron::crypto;

int main() {
  const Key key {};
  const auto nonce1 = NonceGenerator::generate();
  const auto nonce2 = NonceGenerator::generate();
}
