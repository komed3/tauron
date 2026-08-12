#include <chrono>
#include <cstdint>
#include <iomanip>
#include <iostream>
#include <vector>

#include "tauron/crypto/cipher.hpp"

using namespace tauron::crypto;
using Clock = std::chrono::steady_clock;

int main() {
  Key key {};

  for ( std::size_t i = 0; i < key.size(); ++i )
    key[ i ] = static_cast< std::uint8_t >( i );
}
