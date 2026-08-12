#include <chrono>
#include <cstdint>
#include <iomanip>
#include <iostream>
#include <vector>

#include "tauron/core/block.hpp"
#include "tauron/crypto/cipher.hpp"
#include "tauron/crypto/keygen.hpp"
#include "tauron/utils/random.hpp"

using namespace tauron::core;
using namespace tauron::crypto;
using namespace tauron::utils;

using Clock = std::chrono::steady_clock;

constexpr std::size_t BLOCKS = 10'000'000;
constexpr std::size_t ROUNDS = 16;

int main() {
  Key key {};

  for ( std::size_t i = 0; i < key.size(); ++i )
    key[ i ] = static_cast< std::uint8_t >( i );

  const auto nonce = Random::nonce();
  const auto keys = KeyGen::expand( key, nonce, ROUNDS );

  std::vector< DataBlock > blocks( BLOCKS );

  for ( std::size_t i = 0; i < BLOCKS; ++i )
    for ( std::size_t j = 0; j < BLOCK_SIZE; ++j )
      blocks[ i ][ j ] = static_cast< std::uint8_t >( i + j );

  volatile std::uint8_t sink = 0;
  const auto start = Clock::now();

  for ( auto& block : blocks ) {
    block = Cipher::encrypt( block, keys );
    sink ^= block[ 0 ];
  }

  const auto end = Clock::now();
  const auto elapsed = std::chrono::duration_cast< std::chrono::microseconds >( end - start );

  const double seconds = elapsed.count() / 1'000'000.0;
  const double bytes = static_cast< double >( BLOCKS * BLOCK_SIZE );

  const double megabytes = bytes / ( 1024.0 * 1024.0 );
  const double throughput = megabytes / seconds;
  const double blocksPerSecond = BLOCKS / seconds;

  return 0;
}
