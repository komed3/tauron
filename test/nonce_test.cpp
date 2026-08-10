#include <iostream>

#include "tauron/crypto/nonce_generator.hpp"

using namespace tauron::crypto;

int main() {
  const auto nonce1 = NonceGenerator::generate();
  const auto nonce2 = NonceGenerator::generate();

  bool equal = nonce1 == nonce2;
  bool zero = true;

  for ( const auto byte : nonce1 ) if ( byte != 0 ) {
    zero = false;
    break;
  }

  std::cout << "Nonce size: " << nonce1.size() << '\n';
  std::cout << "Nonce 1 == Nonce 2: " << std::boolalpha << equal << '\n';
  std::cout << "Nonce 1 all zero: " << zero << '\n';

  return ( nonce1.size() == NONCE_SIZE && ! equal && ! zero ) ? 0 : 1;
}
