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
  std::cout << "    Ciphertext: ";
  printHex( encrypted );

  std::cout << "    Ciphertext "
            << ( expectDifferent ? "changed" : "stable" )
            << ": "
            << ( encryptionValid ? "PASS" : "FAIL" )
            << '\n';

  std::cout << "    Decryption: "
            << ( decryptionValid ? "PASS" : "FAIL" )
            << '\n';

  std::cout << "    Result: "
            << ( passed ? "PASS" : "FAIL" )
            << "\n\n";

  return passed;
}

} // namespace

int main() {
  std::cout << "=== Tauron Crypto Pipeline ===\n\n";

  const std::string passphrase = "Tauron test passphrase";
  Block block{};

  for ( std::size_t i = 0; i < block.size(); ++i )
    block[ i ] = static_cast< std::uint8_t >( i );

  std::cout << "    Plaintext:  ";
  printHex( block );
  std::cout << "\n";

  /*
   * Baseline
   *
   * Passphrase
   *  -> Salt
   *  -> Master Key
   *  -> Nonce
   *  -> Key Schedule
   *  -> Encryption
   *  -> Decryption
   */

  const auto salt = NonceGenerator::generate();
  const auto nonce = NonceGenerator::generate();
  const auto key = MasterKeyGenerator::generate( passphrase, salt );
  const auto keys = KeySchedule::expand( key, nonce, 16 );
  const auto encrypted = BlockCipher::encrypt( block, keys );
  const auto decrypted = BlockCipher::decrypt( encrypted, keys );

  const bool baselinePassed = decrypted == block;
  bool allPassed = baselinePassed;

  std::cout << "[1] Baseline\n";
  std::cout << "    Ciphertext: ";
  printHex( encrypted );

  std::cout << "    Decryption: "
            << ( baselinePassed ? "PASS" : "FAIL" )
            << "\n\n";

  /*
   * Determinism
   *
   * Identical inputs must produce identical ciphertext.
   */

  const auto repeatedKeys = KeySchedule::expand( key, nonce, 16 );
  const auto repeatedCiphertext = BlockCipher::encrypt( block, repeatedKeys );

  const bool deterministicPassed = repeatedCiphertext == encrypted;
  allPassed &= deterministicPassed;

  std::cout << "[2] Determinism\n";
  std::cout << "    Ciphertext: ";
  printHex( repeatedCiphertext );

  std::cout
        << "    Identical input -> identical ciphertext: "
        << ( deterministicPassed ? "PASS" : "FAIL" )
        << "\n\n";

  /*
   * Different nonce
   *
   * Same key and plaintext, different nonce must produce
   * different ciphertext while remaining decryptable.
   */

  const auto changedNonce = NonceGenerator::generate();

  allPassed &= testPipeline(
    "[3] Changed nonce", passphrase, salt, changedNonce,
    block, encrypted, true
  );

  /*
   * Different salt
   *
   * Same passphrase, nonce and plaintext, different salt must
   * result in a different master key and ciphertext.
   */

  const auto changedSalt = NonceGenerator::generate();

  allPassed &= testPipeline(
    "[4] Changed salt", passphrase, changedSalt, nonce,
    block, encrypted, true
  );

  /*
   * Different passphrase
   *
   * Same salt, nonce and plaintext, different passphrase must
   * result in a different master key and ciphertext.
   */

  allPassed &= testPipeline(
    "[5] Changed passphrase", "Different Tauron test passphrase",
    salt, nonce, block, encrypted, true
  );
}
