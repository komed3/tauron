#include <iostream>

#include "tauron/crypto/key_schedule.hpp"
#include "tauron/crypto/nonce_generator.hpp"

using namespace tauron::crypto;

int main() {
  const std::size_t rounds = 16;

  const Key key {};
  const auto nonce1 = NonceGenerator::generate();
  const auto nonce2 = NonceGenerator::generate();

  const auto keys1 = KeySchedule::expand( key, nonce1, rounds );
  const auto keys1Again = KeySchedule::expand( key, nonce1, rounds );
  const auto keys2 = KeySchedule::expand( key, nonce2, rounds );

  const bool sameNonceSameKeys = keys1 == keys1Again;
  const bool differentNonceDifferentKeys = keys1 != keys2;

  bool firstRoundDiffers = false;

  if ( ! keys1.empty() && ! keys2.empty() )
    firstRoundDiffers = keys1[ 0 ] != keys2[ 0 ];

  std::cout << "Round keys: "
            << keys1.size()
            << '\n';

  std::cout << "Same key + nonce: "
            << std::boolalpha
            << sameNonceSameKeys
            << '\n';

  std::cout << "Different nonce: "
            << differentNonceDifferentKeys
            << '\n';

  std::cout << "First round differs: "
            << firstRoundDiffers
            << '\n';
}
