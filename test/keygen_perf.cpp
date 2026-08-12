#include <chrono>
#include <cstdint>
#include <iomanip>
#include <iostream>
#include <vector>

#include "tauron/core/block.hpp"
#include "tauron/crypto/keygen.hpp"
#include "tauron/utils/random.hpp"

using namespace tauron::core;
using namespace tauron::crypto;
using namespace tauron::utils;

using Clock = std::chrono::steady_clock;

constexpr std::size_t KEYSET = 1'000'000;
constexpr std::size_t ROUNDS = 16;

int main() {
  const auto nonce = Random::nonce();

  std::vector< Key > keys( KEYSET );

  for ( std::size_t i = 0; i < KEYSET; ++i )
    for ( std::size_t j = 0; j < KEY_SIZE; ++j )
      keys[ i ][ j ] = static_cast< std::uint8_t >( i + j );

  volatile std::uint8_t sink = 0;
  const auto start = Clock::now();

  for ( auto& key : keys ) {
    const auto keyset = KeyGen::expand( key, nonce, ROUNDS );
    sink ^= keyset[ 0 ][ 0 ];
  }

  const auto end = Clock::now();
  const auto elapsed = std::chrono::duration_cast< std::chrono::microseconds >( end - start );

  const double seconds = elapsed.count() / 1'000'000.0;
  const double bytes = static_cast< double >( KEYSET * ROUNDS * KEY_SIZE );

  const double megabytes = bytes / ( 1024.0 * 1024.0 );
  const double throughput = megabytes / seconds;
  const double keysetsPerSecond = KEYSET / seconds;

  std::cout << std::fixed << std::setprecision( 2 );

  std::cout << "Key sets:   "
            << KEYSET
            << '\n';

  std::cout << "Rounds:     "
            << ROUNDS
            << '\n';

  std::cout << "Time:       "
            << seconds
            << " s\n";

  std::cout << "Key sets/s: "
            << keysetsPerSecond
            << '\n';

  std::cout << "Throughput: "
            << throughput
            << " MiB/s\n";

  std::cout << "Sink:       "
            << static_cast< unsigned >( sink )
            << '\n';

  return 0;
}
