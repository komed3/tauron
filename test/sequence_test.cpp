#include <cstddef>
#include <cstdint>
#include <algorithm>
#include <array>
#include <iomanip>
#include <iostream>
#include <stdexcept>
#include <string>
#include <vector>

#include "tauron/core/block.hpp"
#include "tauron/core/constants.hpp"
#include "tauron/core/sequence.hpp"

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

static bool throws( const auto& function ) {
  try {
    function();
    return false;
  } catch ( const std::exception& ) {
    return true;
  }
}

static bool validIds( const SequenceResult& result ) {
  std::array< bool, SEQ_BLOCKS > seen {};

  for ( std::size_t i = 0; i < result.count; ++i ) {
    const auto parsed = Block::parse( result.blocks[ i ] );

    if ( parsed.flag != BlockFlag::PASSED ) return false;
    if ( parsed.id >= result.count ) return false;
    if ( seen[ parsed.id ] ) return false;

    seen[ parsed.id ] = true;
  }

  for ( std::size_t i = 0; i < result.count; ++i )
    if ( ! seen[ i ] ) return false;

  return true;
}

static bool roundtrip( const std::span< const std::uint8_t > input, const bool eof ) {
  const auto result = Sequence::build( input, eof );

  std::array< std::uint8_t, SEQ_BLOCKS * BLOCK_PAYLOAD > output {};
  const auto size = Sequence::parse( std::span( result.blocks ).first( result.count ), output );

  return size == input.size() && std::equal( input.begin(), input.end(), output.begin() );
}

int main() {
  bool allPassed = true;

  // 1. Empty EOF sequence

  const std::vector< std::uint8_t > empty {};
  const auto emptyResult = Sequence::build( empty, true );

  const bool emptyCount = emptyResult.count == 0;
  const bool emptyEof = emptyResult.eof;

  printResult( "Empty EOF sequence -> zero blocks", emptyCount );
  printResult( "Empty EOF sequence -> EOF preserved", emptyEof );

  allPassed &= emptyCount && emptyEof;

  // 2. Small EOF sequence

  const std::vector< std::uint8_t > small( 1, 0x42 );
  const auto smallResult = Sequence::build( small, true );

  const bool smallCount = smallResult.count == 1;
  const bool smallIds = validIds( smallResult );
  const bool smallRoundtrip = roundtrip( small, true );

  printResult( "1 byte EOF payload -> one block", smallCount );
  printResult( "Small sequence -> valid IDs", smallIds );
  printResult( "Small sequence roundtrip", smallRoundtrip );

  allPassed &= smallCount && smallIds && smallRoundtrip;

  // 3. Block boundary

  const std::vector< std::uint8_t > exact( BLOCK_PAYLOAD, 0x11 );
  const std::vector< std::uint8_t > over( BLOCK_PAYLOAD + 1, 0x22 );

  const auto exactResult = Sequence::build( exact, true );
  const auto overResult = Sequence::build( over, true );

  const bool exactCount = exactResult.count == 1;
  const bool overCount = overResult.count == 2;
  const bool exactRoundtrip = roundtrip( exact, true );
  const bool overRoundtrip = roundtrip( over, true );

  printResult( "Exactly one block payload -> one block", exactCount );
  printResult( "Payload exceeding one block -> two blocks", overCount );
  printResult( "Exact block boundary roundtrip", exactRoundtrip );
  printResult( "29-byte payload roundtrip", overRoundtrip );

  allPassed &= exactCount && overCount && exactRoundtrip && overRoundtrip;

  // 4. Maximum sequence

  const std::vector< std::uint8_t > maximum( SEQ_BLOCKS * BLOCK_PAYLOAD, 0x37 );
  const auto maximumResult = Sequence::build( maximum, true );

  const bool maximumCount = maximumResult.count == SEQ_BLOCKS;
  const bool maximumIds = validIds( maximumResult );
  const bool maximumRoundtrip = roundtrip( maximum, true );

  printResult( "Maximum EOF payload -> 256 blocks", maximumCount );
  printResult( "Maximum sequence -> valid IDs", maximumIds );
  printResult( "Maximum sequence roundtrip", maximumRoundtrip );

  allPassed &= maximumCount && maximumIds && maximumRoundtrip;

  // 5. Non-EOF sequence is always full

  const std::vector< std::uint8_t > partial( 100, 0x55 );
  const auto fullResult = Sequence::build( partial, false );

  const bool fullCount = fullResult.count == SEQ_BLOCKS;
  const bool fullIds = validIds( fullResult );
  const bool fullRoundtrip = roundtrip( partial, false );

  printResult( "Non-EOF sequence -> always 256 blocks", fullCount );
  printResult( "Non-EOF sequence -> valid IDs", fullIds );
  printResult( "Non-EOF sequence roundtrip", fullRoundtrip );

  allPassed &= fullCount && fullIds && fullRoundtrip;

  // 6. Payload exceeding sequence capacity

  const std::vector< std::uint8_t > oversized( SEQ_BLOCKS * BLOCK_PAYLOAD + 1, 0xAA );
  const bool oversizedThrows = throws( [&] { Sequence::build( oversized, true ); } );

  printResult( "Payload exceeding maximum size -> throw", oversizedThrows );

  allPassed &= oversizedThrows;

  // 7. Invalid block ID
  {
    const auto result = Sequence::build( small, true );
    auto blocks = std::array< DataBlock, 1 > { result.blocks[ 0 ] };
    blocks[ 0 ] = Block::build( 1, small );

    std::array< std::uint8_t, BLOCK_PAYLOAD > output {};

    const bool invalidIdThrows = throws( [&] { Sequence::parse( blocks, output ); } );

    printResult( "Invalid block ID -> throw", invalidIdThrows );

    allPassed &= invalidIdThrows;
  }

  // 8. Duplicate / missing block ID
  {
    const std::vector< std::uint8_t > data( BLOCK_PAYLOAD + 1, 0x66 );
    const auto result = Sequence::build( data, true );

    std::array< DataBlock, 2 > blocks { result.blocks[ 0 ], result.blocks[ 1 ] };
    const auto first = Block::parse( blocks[ 0 ] );
    blocks[ 1 ] = Block::build( first.id, std::span< const std::uint8_t >( data ).subspan( BLOCK_PAYLOAD, 1 ) );
    std::array< std::uint8_t, BLOCK_PAYLOAD * 2 > output {};

    const bool duplicateIdThrows = throws( [&] { Sequence::parse( blocks, output ); } );

    printResult( "Duplicate / missing block ID -> throw", duplicateIdThrows );

    allPassed &= duplicateIdThrows;
  }

  // 9. Invalid checksum
  {
    const auto result = Sequence::build( small, true );
    auto block = result.blocks[ 0 ];
    block[ 4 ] ^= 0x01;

    const std::array< DataBlock, 1 > blocks { block };
    std::array< std::uint8_t, BLOCK_PAYLOAD > output {};

    const bool invalidChecksumThrows = throws( [&] { Sequence::parse( blocks, output ); } );

    printResult( "Invalid block checksum -> throw", invalidChecksumThrows );

    allPassed &= invalidChecksumThrows;
  }

  // 10. Invalid length
  {
    const auto result = Sequence::build( small, true );
    auto block = result.blocks[ 0 ];
    block[ 1 ] = BLOCK_PAYLOAD + 1;

    const std::array< DataBlock, 1 > blocks { block };
    std::array< std::uint8_t, BLOCK_PAYLOAD > output {};

    const bool invalidLengthThrows = throws( [&] { Sequence::parse( blocks, output ); } );

    printResult( "Invalid block length -> throw", invalidLengthThrows );

    allPassed &= invalidLengthThrows;
  }

  // 11. Output buffer too small
  {
    const auto result = Sequence::build( small, true );
    std::array< DataBlock, 1 > blocks { result.blocks[ 0 ] };
    std::array< std::uint8_t, BLOCK_PAYLOAD - 1 > output {};

    const bool smallBufferThrows = throws( [&] { Sequence::parse( blocks, output ); } );

    printResult( "Output buffer too small -> throw", smallBufferThrows );

    allPassed &= smallBufferThrows;
  }

  return allPassed ? 0 : 1;
}
