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
  const auto key2 = MasterKeyGenerator::generate( passphrase, salt2 );
  const auto key3 = MasterKeyGenerator::generate( "Different passphrase", salt1 );

  const bool sameSaltSameKey = key1 == key1Again;
  const bool differentSaltDifferentKey = key1 != key2;
  const bool differentPassphraseDifferentKey = key1 != key3;

  const bool allZero = std::all_of( key1.begin(), key1.end(), [] ( const auto byte ) {
    return byte == 0;
  } );

  std::cout << "Key size: "
            << key1.size()
            << "\n";

  std::cout << "Same salt + passphrase: "
            << std::boolalpha
            << sameSaltSameKey
            << '\n';

  std::cout << "Different salt: "
            << differentSaltDifferentKey
            << '\n';

  std::cout << "Different passphrase: "
            << differentPassphraseDifferentKey
            << '\n';

  std::cout << "Key all zero: "
            << allZero
            << '\n';

  return (
    key1.size() == KEY_SIZE && sameSaltSameKey && differentSaltDifferentKey &&
    differentPassphraseDifferentKey && ! allZero
  ) ? 0 : 1;
}
