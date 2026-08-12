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

constexpr std::size_t KEYGEN = 1'000'000;
constexpr std::size_t ROUNDS = 16;

int main() {
  const auto nonce = Random::nonce();

  std::vector< Key > keyset( KEYGEN );

  for ( std::size_t i = 0; i < KEYGEN; ++i )
    for ( std::size_t j = 0; j < KEY_SIZE; ++j )
      keyset[ i ][ j ] = static_cast< std::uint8_t >( i + j );

  volatile std::uint8_t sink = 0;
  const auto start = Clock::now();

  for ( auto& key : keyset ) {
    const auto keys = KeyGen::expand( key, nonce, ROUNDS );
    sink ^= keys[ 0 ][ 0 ];
  }

  const auto end = Clock::now();
  const auto elapsed = std::chrono::duration_cast< std::chrono::microseconds >( end - start );
}
