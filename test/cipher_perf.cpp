#include <chrono>
#include <cstdint>
#include <iomanip>
#include <iostream>
#include <vector>

#include "tauron/crypto/cipher.hpp"
#include "tauron/crypto/keygen.hpp"
#include "tauron/utils/random.hpp"

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
}
