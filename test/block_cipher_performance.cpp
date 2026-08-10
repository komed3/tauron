#include <chrono>
#include <cstdint>
#include <iomanip>
#include <iostream>
#include <vector>

#include "tauron/crypto/block_cipher.hpp"
#include "tauron/crypto/key_schedule.hpp"
#include "tauron/crypto/nonce_generator.hpp"

using namespace tauron::crypto;
using Clock = std::chrono::steady_clock;

constexpr std::size_t BLOCKS = 1'000'000;
constexpr std::size_t ROUNDS = 16;

int main() {
  Key key {};

  for ( std::size_t i = 0; i < key.size(); ++i )
    key[ i ] = static_cast< std::uint8_t >( i );

  const auto nonce = NonceGenerator::generate();
  const auto keys = KeySchedule::expand( key, nonce, ROUNDS );

  std::vector< Block > blocks( BLOCKS );

  for ( std::size_t i = 0; i < BLOCKS; ++i )
    for ( std::size_t j = 0; j < BLOCK_SIZE; ++j )
      blocks[ i ][ j ] = static_cast< std::uint8_t >( i + j );

  volatile std::uint8_t sink = 0;
  const auto start = Clock::now();

  for ( auto& block : blocks ) {
    block = BlockCipher::encrypt( block, keys );
    sink ^= block[ 0 ];
  }

  const auto end = Clock::now();
  const auto elapsed = std::chrono::duration_cast< std::chrono::microseconds >( end - start );

  const double seconds = elapsed.count() / 1'000'000.0;
  const double bytes = static_cast< double >( BLOCKS * BLOCK_SIZE );

  const double megabytes = bytes / ( 1024.0 * 1024.0 );
  const double throughput = megabytes / seconds;
  const double blocksPerSecond = BLOCKS / seconds;
}
