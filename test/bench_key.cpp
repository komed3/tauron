#include "tauron/crypto/key_schedule.hpp"

#include <chrono>
#include <cstddef>
#include <cstdint>
#include <iomanip>
#include <iostream>

using namespace tauron::crypto;

int main() {
  constexpr std::size_t ROUNDS = 128;
  constexpr std::size_t ITERATIONS = 10000;

  Key key {};

  for ( std::size_t i = 0; i < key.size(); ++i )
    key[ i ] = static_cast< std::uint8_t >( i );

  std::uint64_t checksum = 0;
  const auto start = std::chrono::steady_clock::now();

  for ( std::size_t i = 0; i < ITERATIONS; ++i ) {
    const auto keys = KeySchedule::expand( key, ROUNDS );

    for ( const auto byte : keys.back() )
      checksum = ( checksum * 131 ) ^ byte;
  }

  const auto end = std::chrono::steady_clock::now();
  const auto elapsed = std::chrono::duration_cast< std::chrono::microseconds >( end - start );

  const double totalSeconds = elapsed.count() / 1'000'000.0;
  const double keysPerSecond = ITERATIONS / totalSeconds;
  const double roundsPerSecond = keysPerSecond * ROUNDS;

  std::cout << "KEY SCHEDULE BENCHMARK\n\n";
  std::cout << "Rounds             " << ROUNDS << '\n';
  std::cout << "Iterations         " << ITERATIONS << '\n';
  std::cout << "Total time         " << elapsed.count() << " us\n";
  std::cout << "Per expansion      "
            << ( elapsed.count() / static_cast< double >( ITERATIONS ) )
            << " us\n";
  std::cout << "Expansions/sec     " << keysPerSecond << '\n';
  std::cout << "Round keys/sec     " << roundsPerSecond << '\n';
  std::cout << "Checksum           "
            << std::hex << std::setw( 16 ) << std::setfill( '0' )
            << checksum << '\n';
}
