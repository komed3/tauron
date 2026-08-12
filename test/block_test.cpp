#include <cstdint>
#include <iomanip>
#include <iostream>
#include <span>
#include <vector>

#include "tauron/constants.hpp"
#include "tauron/core/block.hpp"

using namespace tauron;
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
    const auto parsed = Block::parse( block, 255 );
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

  // 2. Empty payload
  {
    const std::vector< std::uint8_t > empty;
    const auto block = Block::build( 0x09, empty );
    const auto parsed = Block::parse( block, 32 );
    const bool passed = parsed.flag == BlockFlag::PASSED && parsed.id == 0x09 && parsed.payload.empty();

    printResult( "Empty payload", passed );
    allPassed &= passed;
  }

  // 3. Maximum payload
  {
    const std::vector< std::uint8_t > maximum( BLOCK_PAYLOAD, 0xAA );
    const auto block = Block::build( 0x1F, maximum );
    const auto parsed = Block::parse( block, 32 );
    const bool passed = parsed.flag == BlockFlag::PASSED && parsed.id == 0x1F && parsed.payload == maximum;

    printResult( "Maximum payload", passed );
    allPassed &= passed;
  }

  // 4. Random block
  {
    const std::vector< std::uint8_t > empty;
    const auto block = Block::build( 0xFF, empty );
    const auto parsed = Block::parse( block, 32 );
    const bool passed = parsed.flag == BlockFlag::PASSED && parsed.id == 0xFF && parsed.payload.empty();

    printResult( "Random block", passed );
    allPassed &= passed;
  }

  // 5. ID at sequence boundary
  {
    const auto block = Block::build( 0x1F, payload );
    const auto parsed = Block::parse( block, 32 );
    const bool passed = parsed.flag == BlockFlag::PASSED;

    printResult( "Maximum valid ID for 32-block sequence", passed );
    allPassed &= passed;
  }

  // 6. ID outside sequence
  {
    const auto block = Block::build( 0x20, payload );
    const auto parsed = Block::parse( block, 32 );
    const bool passed = parsed.flag == BlockFlag::INVALID_ID;

    printResult( "ID outside sequence", passed );
    allPassed &= passed;
  }

  // 7. Sequence size 255
  {
    const auto block = Block::build( 0xFE, payload );
    const auto parsed = Block::parse( block, 255 );
    const bool passed = parsed.flag == BlockFlag::PASSED && parsed.id == 0xFE;

    printResult( "Maximum sequence size", passed );
    allPassed &= passed;
  }

  // 8. Invalid length
  {
    auto block = Block::build( 0x00, payload );
    block[ 1 ] = BLOCK_PAYLOAD + 1;

    const auto parsed = Block::parse( block, 32 );
    const bool passed = parsed.flag == BlockFlag::INVALID_LENGTH;

    printResult( "Invalid payload length", passed );
    allPassed &= passed;
  }

  // 9. Invalid checksum
  {
    auto block = Block::build( 0x00, payload );
    block[ 2 ] ^= 0x01;

    const auto parsed = Block::parse( block, 32 );
    const bool passed = parsed.flag == BlockFlag::INVALID_CHECKSUM;

    printResult( "Modified payload", passed );
    allPassed &= passed;
  }

  // 10. Invalid checksum in random padding
  {
    auto block = Block::build( 0x00, payload );
    block[ 2 + payload.size() ] ^= 0x01;

    const auto parsed = Block::parse( block, 32 );
    const bool passed = parsed.flag == BlockFlag::INVALID_CHECKSUM;

    printResult( "Modified random padding", passed );
    allPassed &= passed;
  }

  // 11. Payload > maximum
  {
    bool threw = false;
    const std::vector< std::uint8_t > oversized( BLOCK_PAYLOAD + 1, 0xAA );

    try { Block::build( 0x00, oversized ); }
    catch ( const std::invalid_argument& ) { threw = true; }

    printResult( "Payload > 28 bytes rejected", threw );
    allPassed &= threw;
  }

  // 12. Random block with payload
  {
    bool threw = false;

    try { Block::build( 0xFF, payload ); }
    catch ( const std::invalid_argument& ) { threw = true; }

    printResult( "Random block with payload rejected", threw );
    allPassed &= threw;
  }

  return allPassed ? 0 : 1;
}
