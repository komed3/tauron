#include <cstddef>
#include <iomanip>
#include <iostream>
#include <string>

#include "tauron/core/sequence.hpp"
#include "tauron/crypto/cipher.hpp"
#include "tauron/crypto/keygen.hpp"
#include "tauron/utils/random.hpp"
#include "tauron/utils/word.hpp"

using namespace tauron::core;
using namespace tauron::crypto;
using namespace tauron::utils;

static void printHex( const auto& data ) {
  for ( const auto byte : data )
    std::cout << std::hex << std::setw( 2 ) << std::setfill( '0' )
              << static_cast< unsigned int >( byte ) << " ";

  std::cout << std::dec << "\n";
}

int main() {
  const std::string passphrase = "Tauron test passphrase";
  const std::size_t rounds = 16;

  const std::string text =
    "Tauron sequence test payload: "
    "this text is intentionally long enough "
    "to produce several blocks.";

  std::cout << "Passphrase:\n" << passphrase << "\n\n";
  std::cout << "Rounds:\n" << rounds << "\n\n";
  std::cout << "Plaintext:\n" << text << "\n\n";

  const auto nonce = Random::nonce();
  const auto salt = Random::salt();

  std::cout << "Nonce:\n";
  printHex( nonce );

  std::cout << "Salt:\n";
  printHex( salt );

  const auto master = KeyGen::derive( passphrase, salt );
  const auto keys = KeyGen::expand( master, nonce, rounds );

  std::cout << "Master key:\n";
  printHex( master );

  std::cout << "\nRound keys:\n";

  for ( std::size_t r = 0; r < keys.size(); ++r ) {
    Bytes bytes {};
    fromWords( keys[ r ], bytes );
    printHex( bytes );
  }
}
