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
  return 0;
}
