#include "tauron/crypto/keygen.hpp"

#include <sodium.h>
#include <stdexcept>

namespace tauron::crypto {

Key KeyGen::derive( std::string_view passphrase, const utils::Salt& salt ) {
  Key key {};

  auto result = crypto_pwhash(
    key.data(), key.size(), passphrase.data(), passphrase.size(), salt.data(),
    crypto_pwhash_OPSLIMIT_MODERATE, crypto_pwhash_MEMLIMIT_MODERATE, crypto_pwhash_ALG_ARGON2ID13
  );

  if ( result != 0 )
    throw std::runtime_error( "Failed to derive master key" );

  return key;
}

}
