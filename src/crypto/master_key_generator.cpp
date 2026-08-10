#include "tauron/crypto/master_key_generator.hpp"

#include <sodium.h>
#include <stdexcept>

namespace tauron::crypto {

MasterKey MasterKeyGenerator::generate( std::string_view passphrase, const Salt& salt ) {
  MasterKey key {};

  auto result = crypto_pwhash(
    key.data(), key.size(), passphrase.data(), passphrase.size(), salt.data(),
    crypto_pwhash_OPSLIMIT_MODERATE, crypto_pwhash_MEMLIMIT_MODERATE, crypto_pwhash_ALG_ARGON2ID13
  );

  if ( result != 0 )
    throw std::runtime_error( "Failed to derive master key" );

  return key;
}

}
