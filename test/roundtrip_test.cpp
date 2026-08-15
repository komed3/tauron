#include <cstddef>
#include <iomanip>
#include <iostream>
#include <span>
#include <string>
#include <vector>

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

  std::cout << "Passphrase: " << passphrase << "\n";
  std::cout << "Rounds:     " << rounds << "\n\n";

  std::cout << "Plaintext:\n" << text << "\n\n";

  // Generate round keys from passphrase

  const auto nonce = Random::nonce();
  const auto salt = Random::salt();

  std::cout << "Nonce:  ";
  printHex( nonce );

  std::cout << "Salt:   ";
  printHex( salt );

  const auto master = KeyGen::derive( passphrase, salt );
  const auto keys = KeyGen::expand( master, nonce, rounds );

  std::cout << "Master: ";
  printHex( master );

  std::cout << "\nRound keys:\n";

  for ( std::size_t r = 0; r < keys.size(); ++r ) {
    Bytes bytes {};
    fromWords( keys[ r ], bytes );
    printHex( bytes );
  }

  // Build sequence with blocks

  const auto sequence = Sequence::build(
    std::span( reinterpret_cast< const std::uint8_t* >( text.data() ), text.size() ),
    true
  );

  std::cout << "\nBlock count: " << sequence.count;
  std::cout << "\nEOF:         " << ( sequence.eof ? "yes" : "no" );
  std::cout << "\n\nBlocks:\n";

  for ( std::size_t i = 0; i < sequence.count; ++i )
    printHex( sequence.blocks[ i ] );

  // Encrypt blocks

  std::vector< std::uint8_t > encrypted {};

  std::cout << "\nEncrypted blocks:\n";

  for ( std::size_t i = 0; i < sequence.count; ++i ) {
    const auto result = Cipher::encrypt( sequence.blocks[ i ], keys );
    printHex( result );

    encrypted.insert( encrypted.end(), result.begin(), result.end() );
  }

  std::cout << "\nCiphertext:\n";
  printHex( encrypted );

  // Decrypt blocks

  std::size_t count = encrypted.size() / BLOCK_SIZE;
  std::vector< DataBlock > blocks( count );

  std::cout << "\nDecrypted blocks:\n";

  for ( std::size_t i = 0; i < count; ++i ) {
    DataBlock block {};

    for ( std::size_t j = 0; j < BLOCK_SIZE; ++j )
      block[ j ] = encrypted[ i * BLOCK_SIZE + j ];

    blocks[ i ] = Cipher::decrypt( block, keys );
    printHex( blocks[ i ] );
  }

  // Parse sequence

  std::vector< std::uint8_t > payload( count * BLOCK_PAYLOAD );
  Sequence::parse( blocks, payload );

  std::string message( payload.begin(), payload.end() );
  std::cout << "\nDecrypted text:\n" << message;
}
