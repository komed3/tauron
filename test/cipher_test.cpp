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
}
