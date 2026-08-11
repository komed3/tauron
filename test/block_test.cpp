#include <cstdint>
#include <iomanip>
#include <iostream>
#include <span>
#include <vector>

#include "tauron/core/block.hpp"

using namespace tauron::core;

static void printResult( const char* name, const bool passed ) {
  std::cout << "[" << ( passed ? "PASS" : "FAIL" ) << "] " << name << '\n';
}

void printHex( const auto& data ) {
  for ( const auto byte : data )
    std::cout << std::hex << std::setw( 2 ) << std::setfill( '0' )
              << static_cast< unsigned int >( byte ) << " ";

  std::cout << std::dec << "\n";
}

int main() {
  bool allPassed = true;
  const std::vector< std::uint8_t > payload {
    0x00, 0x01, 0x02, 0x03, 0x04, 0x05, 0x06, 0x07,
    0x08, 0x09, 0x0A, 0x0B, 0x0C, 0x0D, 0x0E, 0x0F
  };

  // 1. Normal block
  {
    const auto block = Block::build( 0x2B, payload );
    const auto parsed = Block::parse( block, 32 );
    const bool passed = parsed.flag == BlockFlag::PASSED && parsed.id == 0x2B && parsed.payload == payload;

    std::cout << "Payload: ";
    printHex( payload );

    std::cout << "Block:   ";
    printHex( block );

    std::cout << "Parsed:  ";
    printHex( parsed.payload );

    std::cout << "\n";

    printResult( "Normal block roundtrip", passed );
    allPassed &= passed;
  }
}
