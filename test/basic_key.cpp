#include "tauron/crypto/key_schedule.hpp"

#include <iomanip>
#include <iostream>

using namespace tauron::crypto;

void printKey( const Key& key ) {
  for ( const auto byte : key )
    std::cout << std::hex
              << std::setw( 2 )
              << std::setfill( '0' )
              << static_cast< int >( byte )
              << ' ';

  std::cout << std::dec << '\n';
}

bool equal( const Key& a, const Key& b ) {
  return a == b;
}

int main() {
  Key key{};

  for ( std::size_t i = 0; i < key.size(); ++i )
    key[ i ] = static_cast< std::uint8_t >( i );

  constexpr std::size_t rounds = 16;

  const auto keys = KeySchedule::expand( key, rounds );
  const auto keysAgain = KeySchedule::expand( key, rounds );

  std::cout << "KEY SCHEDULE TEST\n\n";

  std::cout << "Rounds       " << rounds << '\n';
  std::cout << "Round keys   " << keys.size() << '\n';
  std::cout << "Expected     " << rounds + 1 << "\n\n";

  std::cout << "Round 0      ";
  printKey( keys.front() );

  std::cout << "Round 1      ";
  printKey( keys[ 1 ] );

  std::cout << "Round " << rounds << "     ";
  printKey( keys.back() );

  const bool sizePass = keys.size() == rounds + 1;
  const bool initialPass = equal( keys.front(), key );
  const bool deterministicPass = keys == keysAgain;

  bool distinctPass = true;

  for ( std::size_t i = 1; i < keys.size(); ++i ) {
    if ( keys[ i ] == keys[ i - 1 ] ) {
      distinctPass = false;
      break;
    }
  }

  std::cout << '\n';

  std::cout << "Key count       " << ( sizePass ? "PASS" : "FAIL" ) << '\n';
  std::cout << "Initial key     " << ( initialPass ? "PASS" : "FAIL" ) << '\n';
  std::cout << "Determinism     " << ( deterministicPass ? "PASS" : "FAIL" ) << '\n';
  std::cout << "Distinct rounds " << ( distinctPass ? "PASS" : "FAIL" ) << '\n';

  return sizePass && initialPass && deterministicPass && distinctPass ? 0 : 1;
}
