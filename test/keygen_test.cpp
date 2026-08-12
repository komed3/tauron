#include <algorithm>
#include <iomanip>
#include <iostream>
#include <string>

#include "tauron/crypto/keygen.hpp"
#include "tauron/utils/random.hpp"
#include "tauron/utils/word.hpp"

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

  // 1. Key derivation

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

  std::cout << "Salt:       ";
  printHex( salt1 );

  std::cout << "Master key: ";
  printHex( key1 );

  std::cout << "\n";

  printResult( "Same salt + passphrase -> same key", sameSaltSameKey );
  printResult( "Different salt -> different key", diffSaltDiffKey );
  printResult( "Different passphrase -> different key", diffPassphraseDiffKey );
  printResult( "Key not all zero", notAllZero );

  allPassed &= sameSaltSameKey && diffSaltDiffKey && diffPassphraseDiffKey && notAllZero;

  // 2. Key expansion

  const auto nonce1 = Random::nonce();
  const auto nonce2 = Random::nonce();

  const auto keys1 = KeyGen::expand( key1, nonce1, 16 );
  const auto keys1Again = KeyGen::expand( key1, nonce1, 16 );
  const auto keys2 = KeyGen::expand( key1, nonce2, 16 );
  const auto keys3 = KeyGen::expand( key2, nonce1, 16 );
  const auto keys4 = KeyGen::expand( key3, nonce1, 16 );

  const bool sameNonceSameKeys = keys1 == keys1Again;
  const bool diffNonceDiffKeys = keys1 != keys2;
  const bool diffMasterDiffKeys = keys1 != keys3;
  const bool diffPassphraseDiffKeys = keys1 != keys4;

  bool allRoundsDiffer = true;

  for ( std::size_t r = 0; r < keys1.size(); ++r ) {
    allRoundsDiffer &=
      keys1[ r ] != keys2[ r ] && keys1[ r ] != keys3[ r ] && keys1[ r ] != keys4[ r ] &&
      keys2[ r ] != keys3[ r ] && keys2[ r ] != keys4[ r ] && keys3[ r ] != keys4[ r ];
  }

  std::cout << "\n\n";

  for ( std::size_t r = 0; r < keys1.size(); ++r ) {
    std::cout << "Round " << std::setfill( ' ' ) << std::setw( 2 ) << ( r + 1 ) << ":   ";

    Bytes bytes {};
    fromWords( keys1[ r ], bytes );
    printHex( bytes );
  }

  std::cout << "\n";

  printResult( "Same nonce + master key -> same keys", sameNonceSameKeys );
  printResult( "Different nonce -> different keys", diffNonceDiffKeys);
  printResult( "Different master key -> different keys", diffMasterDiffKeys );
  printResult( "Different passphrase -> different keys", diffPassphraseDiffKeys );
  printResult( "All round keys differ", allRoundsDiffer );

  allPassed &=
    sameNonceSameKeys && diffNonceDiffKeys && diffMasterDiffKeys &&
    diffPassphraseDiffKeys && allRoundsDiffer;

  std::cout << "\nResult:     "
            << ( allPassed ? "PASS" : "FAIL" )
            << "\n";

  return allPassed ? 0 : 1;
}
