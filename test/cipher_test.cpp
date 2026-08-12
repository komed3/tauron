#include <cstddef>
#include <cstdint>
#include <iomanip>
#include <iostream>

#include "tauron/core/block.hpp"
#include "tauron/crypto/cipher.hpp"
#include "tauron/crypto/keygen.hpp"
#include "tauron/utils/random.hpp"

using namespace tauron::core;
using namespace tauron::crypto;
using namespace tauron::utils;

static void printResult( const char* name, const bool passed ) {
  std::cout << "[" << ( passed ? "PASS" : "FAIL" ) << "] " << name << '\n';
}

void printHex( const auto& data ) {
  for ( const auto byte : data )
    std::cout << std::hex << std::setw( 2 ) << std::setfill( '0' )
              << static_cast< unsigned int >( byte ) << " ";

  std::cout << std::dec << "\n";
}

int main() {
  Key key {};
  DataBlock block {};

  for ( std::size_t i = 0; i < key.size(); ++i )
    key[ i ] = static_cast< std::uint8_t >( i );

  for ( std::size_t i = 0; i < block.size(); ++i )
    block[ i ] = static_cast< std::uint8_t >( 255 - i );

  std::cout << "Master key: ";
  printHex( key );

  std::cout << "Data block: ";
  printHex( block );

  const auto nonce1 = Random::nonce();
  const auto nonce2 = Random::nonce();

  const auto keys1 = KeyGen::expand( key, nonce1, 16 );
  const auto keys2 = KeyGen::expand( key, nonce2, 16 );

  const auto encrypted1 = Cipher::encrypt( block, keys1 );
  const auto decrypted1 = Cipher::decrypt( encrypted1, keys1 );
  const auto encrypted2 = Cipher::encrypt( block, keys2 );

  const bool encrypted = encrypted1 != block;
  const bool decrypted = decrypted1 == block;
  const bool nonceAffectsCiphertext = encrypted1 != encrypted2;

  std::cout << "Encrypted:  ";
  printHex( encrypted1 );

  std::cout << "Decrypted:  ";
  printHex( decrypted1 );

  std::cout << "\n";

  printResult( "Encrypted differs plaintext", encrypted );
  printResult( "Decrypt restores block", decrypted );
  printResult( "Nonce affects cipher text", nonceAffectsCiphertext );

  bool allPassed = encrypted && decrypted && nonceAffectsCiphertext;

  std::cout << "\nResult:  "
            << ( allPassed ? "PASS" : "FAIL" )
            << "\n";

  return allPassed ? 0 : 1;
}
