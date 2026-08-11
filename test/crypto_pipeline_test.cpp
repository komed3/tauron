#include <array>
#include <cstdint>
#include <iomanip>
#include <iostream>
#include <string>

#include "tauron/crypto/block_cipher.hpp"
#include "tauron/crypto/constants.hpp"
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

bool equal( const auto& a, const auto& b ) {
  return a == b;
}

bool testPipeline(
  const std::string& name, const auto& block, const auto& key, const auto& nonce,
  const auto& expectedCiphertext, bool expectDifferent
) {
  const auto keys = KeySchedule::expand( key, block, 16 );
  const auto encrypted = BlockCipher::encrypt( block, keys );
  const auto decrypted = BlockCipher::decrypt( encrypted, keys );

  std::cout << name << "\n";
  std::cout << "  Ciphertext: ";
  printHex( encrypted );
}

}

int main() {}
