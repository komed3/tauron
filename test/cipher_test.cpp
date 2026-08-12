#include <cstddef>
#include <cstdint>

#include "tauron/core/block.hpp"
#include "tauron/crypto/cipher.hpp"
#include "tauron/crypto/keygen.hpp"
#include "tauron/utils/random.hpp"

using namespace tauron::core;
using namespace tauron::crypto;
using namespace tauron::utils;

int main() {
  Key key {};
  DataBlock block {};

  for ( std::size_t i = 0; i < key.size(); ++i )
    key[ i ] = static_cast< std::uint8_t >( i );

  for ( std::size_t i = 0; i < block.size(); ++i )
    block[ i ] = static_cast< std::uint8_t >( i + 32 );

  const auto nonce1 = Random::nonce();
  const auto nonce2 = Random::nonce();

  const auto keys1 = KeyGen::expand( key, nonce1, 16 );
  const auto keys2 = KeyGen::expand( key, nonce2, 16 );
}
