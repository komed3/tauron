#include <algorithm>
#include <cstdint>
#include <iostream>

#include "tauron/crypto/block_cipher.hpp"
#include "tauron/crypto/key_schedule.hpp"
#include "tauron/crypto/nonce_generator.hpp"

using namespace tauron::crypto;

int main() {
  Key key {};
  Block block {};

  for ( std::size_t i = 0; i < key.size(); ++i )
    key[ i ] = static_cast< std::uint8_t >( i );

  for ( std::size_t i = 0; i < block.size(); ++i )
    block[ i ] = static_cast< std::uint8_t >( i + 32 );
}
