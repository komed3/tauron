#include <algorithm>
#include <iomanip>
#include <iostream>
#include <string>

#include "tauron/crypto/keygen.hpp"
#include "tauron/utils/random.hpp"

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
  bool allPassed = true;

  const std::string passphrase = "Tauron test passphrase";
  const auto salt1 = Random::salt();
  const auto salt2 = Random::salt();

  const auto key1 = KeyGen::derive( passphrase, salt1 );
  const auto key1Again = KeyGen::derive( passphrase, salt1 );
  const auto key2 = KeyGen::derive( passphrase, salt2 );
  const auto key3 = KeyGen::derive( "Different passphrase", salt1 );

  const bool sameSaltSameKey = key1 == key1Again;
  const bool diffSaltDiffKey = key1 != key2;
  const bool diffPassphraseDiffKey = key1 != key3;

  const bool notAllZero = std::all_of( key1.begin(), key1.end(), [] ( const auto byte ) {
    return byte != 0;
  } );

  allPassed &= sameSaltSameKey && diffSaltDiffKey && diffPassphraseDiffKey && notAllZero;

  printResult( "Same salt + passphrase -> same key", sameSaltSameKey );
  printResult( "Different salt -> different key", diffSaltDiffKey );
  printResult( "Different passphrase -> different key", diffPassphraseDiffKey );
  printResult( "Key not all zero", notAllZero );

  std::cout << "\nResult:  "
            << ( allPassed ? "PASS" : "FAIL" )
            << "\n";

  return allPassed ? 0 : 1;
}
