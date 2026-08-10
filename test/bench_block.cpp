#include "tauron/crypto/block_cipher.hpp"
#include "tauron/crypto/key_schedule.hpp"

#include <chrono>
#include <cstdint>
#include <iomanip>
#include <iostream>

using namespace tauron::crypto;

int main() {
  constexpr std::size_t ROUNDS = 16;
  constexpr std::size_t ITERATIONS = 1'000'000;

  Key key {};

  for ( std::size_t i = 0; i < key.size(); ++i )
    key[ i ] = static_cast< std::uint8_t >( i );

  Block block {};

  for ( std::size_t i = 0; i < block.size(); ++i )
    block[ i ] = static_cast< std::uint8_t >( i * 3 + 1 );

  const auto keys = KeySchedule::expand( key, ROUNDS );

  std::uint64_t checksum = 0;
  const auto start = std::chrono::steady_clock::now();

  for ( std::size_t i = 0; i < ITERATIONS; ++i ) {
    const auto encrypted = BlockCipher::encrypt( block, keys );
    for ( const auto byte : encrypted ) checksum = ( checksum * 131 ) ^ byte;
  }

  const auto end = std::chrono::steady_clock::now();
  const auto elapsed = std::chrono::duration_cast< std::chrono::microseconds >( end - start );

  const double totalSeconds = elapsed.count() / 1'000'000.0;
  const double blocksPerSecond = ITERATIONS / totalSeconds;
  const double roundsPerSecond = blocksPerSecond * ROUNDS;

  std::cout << "BLOCK CIPHER BENCHMARK\n\n";
  std::cout << "Rounds             " << ROUNDS << '\n';
  std::cout << "Iterations         " << ITERATIONS << '\n';
  std::cout << "Total time         " << elapsed.count() << " us\n";
  std::cout << "Per block          "
            << ( elapsed.count() / static_cast< double >( ITERATIONS ) )
            << " us\n";
  std::cout << "Blocks/sec         " << blocksPerSecond << '\n';
  std::cout << "Rounds/sec         " << roundsPerSecond << '\n';
  std::cout << "Checksum           "
            << std::hex << std::setw( 16 ) << std::setfill( '0' )
            << checksum << '\n';
}
