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

  const std::string text =
    "Tauron sequence test payload: "
    "this text is intentionally long enough "
    "to produce several blocks.";

  //
}
