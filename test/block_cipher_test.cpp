#include <algorithm>
#include <cstdint>
#include <iostream>

#include "tauron/crypto/block_cipher.hpp"
#include "tauron/crypto/key_schedule.hpp"
#include "tauron/crypto/nonce_generator.hpp"

using namespace tauron::crypto;

int main() {
  const std::size_t rounds = 16;

  Key key {};
  Block block {};

  for ( std::size_t i = 0; i < key.size(); ++i )
    key[ i ] = static_cast< std::uint8_t >( i );

  for ( std::size_t i = 0; i < block.size(); ++i )
    block[ i ] = static_cast< std::uint8_t >( i + 32 );

  const auto nonce1 = NonceGenerator::generate();
  const auto nonce2 = NonceGenerator::generate();

  const auto keys1 = KeySchedule::expand( key, nonce1, rounds );
  const auto keys2 = KeySchedule::expand( key, nonce2, rounds );

  const auto encrypted1 = BlockCipher::encrypt( block, keys1 );
  const auto decrypted1 = BlockCipher::decrypt( encrypted1, keys1 );

  const auto encrypted2 = BlockCipher::encrypt( block, keys2 );
}
