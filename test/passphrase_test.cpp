#include <algorithm>
#include <iostream>
#include <string>

#include "tauron/crypto/master_key_generator.hpp"
#include "tauron/crypto/nonce_generator.hpp"

using namespace tauron::crypto;

int main () {
  const std::string passphrase = "Tauron test passphrase";

  const auto salt1 = NonceGenerator::generate();
  const auto salt2 = NonceGenerator::generate();

  const auto key1 = MasterKeyGenerator::generate( passphrase, salt1 );
  const auto key1Again = MasterKeyGenerator::generate( passphrase, salt1 );
}
