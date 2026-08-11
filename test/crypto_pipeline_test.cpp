#include <cstdint>
#include <iomanip>
#include <iostream>
#include <string>

#include "tauron/crypto/block_cipher.hpp"
#include "tauron/crypto/key_schedule.hpp"
#include "tauron/crypto/master_key_generator.hpp"
#include "tauron/crypto/nonce_generator.hpp"

using namespace tauron::crypto;

namespace {

void printHex( const auto& data ) {
  for ( const auto byte : data )
    std::cout << std::hex << std::setw( 2 ) << std::setfill( '0' )
              << static_cast< unsigned int >( byte ) << " ";

  std::cout << std::dec << "\n";
}

bool testPipeline(
  const std::string& name, const std::string& passphrase, const auto& salt, const auto& nonce,
    const auto& block, const auto& referenceCiphertext, bool expectDifferent
) {
  const auto key = MasterKeyGenerator::generate( passphrase, salt );
  const auto keys = KeySchedule::expand( key, nonce, 16 );
  const auto encrypted = BlockCipher::encrypt( block, keys );
  const auto decrypted = BlockCipher::decrypt( encrypted, keys );

  const bool encryptionValid = expectDifferent ? encrypted != referenceCiphertext : encrypted == referenceCiphertext;
  const bool decryptionValid = decrypted == block;
  const bool passed = encryptionValid && decryptionValid;

  std::cout << name << "\n";
  std::cout << "  Ciphertext: ";
  printHex( encrypted );

  std::cout << "  Ciphertext "
            << ( expectDifferent ? "changed" : "stable" )
            << ": "
            << ( encryptionValid ? "PASS" : "FAIL" )
            << '\n';

  std::cout << "  Decryption: "
            << ( decryptionValid ? "PASS" : "FAIL" )
            << '\n';

  std::cout << "  Result: "
            << ( passed ? "PASS" : "FAIL" )
            << "\n\n";

  return passed;
}

} // namespace

int main() {
  //
}
