#include "tauron/crypto/block_cipher.hpp"
#include "tauron/crypto/key_schedule.hpp"

#include <array>
#include <cstdint>
#include <iomanip>
#include <iostream>

using namespace tauron::crypto;

void printBlock( const Block& block ) {
  for ( const auto byte : block )
    std::cout << std::hex
              << std::setw( 2 )
              << std::setfill( '0' )
              << static_cast< int >( byte )
              << ' ';

  std::cout << std::dec << '\n';
}

int main() {
  using namespace tauron::crypto;

  constexpr Key key = {
    0x00, 0x11, 0x22, 0x33,
    0x44, 0x55, 0x66, 0x77,
    0x88, 0x99, 0xaa, 0xbb,
    0xcc, 0xdd, 0xee, 0xff,
    0x10, 0x32, 0x54, 0x76,
    0x98, 0xba, 0xdc, 0xfe,
    0x12, 0x34, 0x56, 0x78,
    0x9a, 0xbc, 0xde, 0xf0
  };

  constexpr Block block = {
    0x01, 0x23, 0x45, 0x67,
    0x89, 0xab, 0xcd, 0xef,
    0x10, 0x32, 0x54, 0x76,
    0x98, 0xba, 0xdc, 0xfe,
    0x11, 0x22, 0x33, 0x44,
    0x55, 0x66, 0x77, 0x88,
    0x99, 0xaa, 0xbb, 0xcc,
    0xdd, 0xee, 0xff, 0x00
  };

  constexpr std::size_t rounds = 16;

  const auto keys = KeySchedule::expand( key, rounds );
  const auto encrypted = BlockCipher::encrypt( block, keys );
  const auto decrypted = BlockCipher::decrypt( encrypted, keys );

  std::cout << "KEY          ";
  printBlock( key );

  std::cout << "BLOCK        ";
  printBlock( block );

  std::cout << "ENCRYPTED    ";
  printBlock( encrypted );

  std::cout << "DECRYPTED    ";
  printBlock( decrypted );

  if ( decrypted != block ) {
    std::cerr << "-> Block cipher test failed\n";
    return 1;
  }

  std::cout << "-> Block cipher test passed\n";
  return 0;
}
