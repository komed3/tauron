#include <algorithm>
#include <iostream>
#include <stdexcept>
#include <string>
#include <vector>

#include "tauron/core/constants.hpp"
#include "tauron/crypto/keygen.hpp"
#include "tauron/stream/worker.hpp"
#include "tauron/utils/random.hpp"

using namespace tauron::crypto;
using namespace tauron::stream;
using namespace tauron::utils;

static void printResult( const char* name, const bool passed ) {
  std::cout << "[" << ( passed ? "PASS" : "FAIL" ) << "] " << name << '\n';
}

static std::vector< std::uint8_t > makePayload( const std::size_t size ) {
  std::vector< std::uint8_t > payload( size );

  for ( std::size_t i = 0; i < size; ++i )
    payload[ i ] = static_cast< std::uint8_t >( i & 0xFF );

  return payload;
}

static bool roundtrip( Worker& worker, const std::vector< std::uint8_t >& payload, const bool eof ) {
  const auto sequencePayload = tauron::core::SEQ_BLOCKS * tauron::core::BLOCK_PAYLOAD;
  const auto sequenceCiphertext = tauron::core::SEQ_BLOCKS * tauron::core::BLOCK_SIZE;
  const auto count = payload.empty() ? 0 : ( payload.size() + sequencePayload - 1 ) / sequencePayload;

  std::vector< std::uint8_t > encrypted( count * sequenceCiphertext );
}

int main() {
  return 0;
}
