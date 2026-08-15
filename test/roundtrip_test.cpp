#include <cstddef>
#include <iostream>
#include <string>

#include "tauron/core/sequence.hpp"
#include "tauron/crypto/cipher.hpp"
#include "tauron/crypto/keygen.hpp"
#include "tauron/utils/random.hpp"

using namespace tauron::core;
using namespace tauron::crypto;
using namespace tauron::utils;

int main() {
  const std::string passphrase = "Tauron test passphrase";
  const std::size_t rounds = 16;

  const std::string text =
    "Tauron sequence test payload: "
    "this text is intentionally long enough "
    "to produce several blocks.";

  std::cout << "Passphrase: " << passphrase << "\n";
  std::cout << "Rounds:     " << rounds << "\n\n";

  std::cout << "Plaintext:\n" << text << "\n\n";

  const auto salt = Random::salt();
  const auto nonce = Random::nonce();

  const auto key = KeyGen::derive( passphrase, salt );
  const auto keys = KeyGen::expand( key, nonce, rounds );
}
