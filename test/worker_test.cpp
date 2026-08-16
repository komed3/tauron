#include <algorithm>
#include <iostream>
#include <stdexcept>
#include <string>
#include <vector>

#include "tauron/core/constants.hpp"
#include "tauron/crypto/keygen.hpp"
#include "tauron/stream/worker.hpp"
#include "tauron/utils/random.hpp"

using namespace tauron::core;
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
  const auto sequencePayload = SEQ_BLOCKS * BLOCK_PAYLOAD;
  const auto sequenceCiphertext = SEQ_BLOCKS * BLOCK_SIZE;
  const auto count = payload.empty() ? 0 : ( payload.size() + sequencePayload - 1 ) / sequencePayload;

  std::vector< std::uint8_t > encrypted( count * sequenceCiphertext );
  const auto encryptedResult = worker.run( Operation::ENCRYPT, payload, encrypted, eof );

  if ( encryptedResult.state != WorkerResultState::COMPLETED ) return false;
  if ( encryptedResult.bytes_read != payload.size() ) return false;

  std::vector< std::uint8_t > decrypted( count * sequencePayload );

  const auto decryptedResult = worker.run(
    Operation::DECRYPT,
    std::span< const std::uint8_t >( encrypted.data(), encryptedResult.bytes_written ),
    decrypted
  );

  if ( decryptedResult.state != WorkerResultState::COMPLETED ) return false;
  if ( decryptedResult.bytes_read != encryptedResult.bytes_written ) return false;
  if ( decryptedResult.bytes_written != payload.size() ) return false;

  return worker.ready() && std::equal( payload.begin(), payload.end(), decrypted.begin() );
}

static bool throws( const auto& function ) {
  try {
    function();
    return false;
  } catch ( const std::exception& ) {
    return true;
  }
}

int main() {
  bool allPassed = true;

  // Test setup

  const std::string passphrase = "Tauron worker test passphrase";
  const auto salt = Random::salt();
  const auto nonce = Random::nonce();

  const auto master = KeyGen::derive( passphrase, salt );
  const auto keys = KeyGen::expand( master, nonce, 16 );

  Worker worker( 1, keys );

  const auto sequencePayload = SEQ_BLOCKS * BLOCK_PAYLOAD;
  const auto sequenceCiphertext = SEQ_BLOCKS * BLOCK_SIZE;
  const auto maxPayload = CHUNK_SEQS * sequencePayload;
  const auto maxCiphertext = CHUNK_SEQS * sequenceCiphertext;

  // 1. Initial state
  {
    const bool passed =
      worker.id() == 1 &&
      worker.ready() &&
      worker.bytesProcessed() == 0 &&
      worker.bytesWritten() == 0;

    printResult( "Worker initial state", passed );
    allPassed &= passed;
  }

  // 2. Empty payload
  {
    std::vector< std::uint8_t > output;
    const auto result = worker.run( Operation::ENCRYPT, {}, output, true );

    const bool passed =
      result.state == WorkerResultState::COMPLETED &&
      result.bytes_read == 0 &&
      result.bytes_written == 0 &&
      worker.bytesProcessed() == 0 &&
      worker.bytesWritten() == 0 &&
      worker.ready();

    printResult( "Empty payload", passed );
    allPassed &= passed;
  }

  // 3. Small EOF payload
  {
    const bool passed =
      roundtrip( worker, makePayload( 1 ), true ) &&
      roundtrip( worker, makePayload( 127 ), true ) &&
      roundtrip( worker, makePayload( 1024 ), true );

    printResult( "Small EOF payloads", passed );
    allPassed &= passed;
  }

  // 4. Block boundaries
  {
    const bool passed =
      roundtrip( worker, makePayload( BLOCK_PAYLOAD - 1 ), true ) &&
      roundtrip( worker, makePayload( BLOCK_PAYLOAD ), true ) &&
      roundtrip( worker, makePayload( BLOCK_PAYLOAD + 1 ), true );

    printResult( "Block payload boundaries", passed );
    allPassed &= passed;
  }

  // 5. Sequence boundaries
  {
    const bool passed =
      roundtrip( worker, makePayload( sequencePayload - 1 ), true ) &&
      roundtrip( worker, makePayload( sequencePayload ), true ) &&
      roundtrip( worker, makePayload( sequencePayload + 1 ), true ) &&
      roundtrip( worker, makePayload( sequencePayload * 2 ), true ) &&
      roundtrip( worker, makePayload( sequencePayload * 2 + 1 ), true );

    printResult( "Sequence boundaries", passed );
    allPassed &= passed;
  }

  // 6. Maximum chunk
  {
    const bool passed =
      roundtrip( worker, makePayload( maxPayload ), false ) &&
      roundtrip( worker, makePayload( maxPayload ), true );

    printResult( "Maximum chunk", passed );
    allPassed &= passed;
  }

  // 7. Maximum chunk minus one byte
  {
    const bool passed = roundtrip( worker, makePayload( maxPayload - 1 ), true );

    printResult( "Maximum EOF chunk", passed );
    allPassed &= passed;
  }

  // 8. Reject incomplete non-EOF payload
  {
    const bool passed = throws( [&] {
      const auto payload = makePayload( sequencePayload );
      std::vector< std::uint8_t > output( sequenceCiphertext );

      worker.run( Operation::ENCRYPT, payload, output, false );
    } ) && worker.ready();

    printResult( "Reject incomplete non-EOF payload", passed );
    allPassed &= passed;
  }

  // 9. Reject payload exceeding maximum chunk
  {
    const bool passed = throws( [&] {
      const auto payload = makePayload( maxPayload + 1 );
      std::vector< std::uint8_t > output( maxCiphertext + sequenceCiphertext );

      worker.run( Operation::ENCRYPT, payload, output, true );
    } ) && worker.ready();

    printResult( "Reject oversized encryption payload", passed );
    allPassed &= passed;
  }

  // 10. Reject undersized encryption buffer
  {
    const bool passed = throws( [&] {
      const auto payload = makePayload( 1000 );
      std::vector< std::uint8_t > output( sequenceCiphertext - 1 );

      worker.run( Operation::ENCRYPT, payload, output, true );
    } ) && worker.ready();

    printResult( "Reject undersized encryption buffer", passed );
    allPassed &= passed;
  }

  // 11. Reject oversized ciphertext
  {
    const bool passed = throws( [&] {
      const auto payload = makePayload( maxCiphertext + 1 );
      std::vector< std::uint8_t > output( maxPayload );

      worker.run( Operation::DECRYPT, payload, output );
    } ) && worker.ready();

    printResult( "Reject oversized ciphertext", passed );
    allPassed &= passed;
  }

  // 12. Reject unaligned ciphertext
  {
    const bool passed = throws( [&] {
      const auto payload = makePayload( tauron::core::BLOCK_SIZE + 1 );
      std::vector< std::uint8_t > output( sequencePayload );

      worker.run( Operation::DECRYPT, payload, output );
    } ) && worker.ready();

    printResult( "Reject unaligned ciphertext", passed );
    allPassed &= passed;
  }

  // 13. Reject undersized decryption buffer
  {
    const bool passed = throws( [&] {
      const auto payload = makePayload( sequenceCiphertext );
      std::vector< std::uint8_t > output( sequencePayload - 1 );

      worker.run( Operation::DECRYPT, payload, output );
    } ) && worker.ready();

    printResult( "Reject undersized decryption buffer", passed );
    allPassed &= passed;
  }

  // 14. Worler reusable after exception
  {
    const bool passed = worker.ready() && roundtrip( worker, makePayload( 4096 ), true );

    printResult( "Worker reusable after exception", passed );
    allPassed &= passed;
  }

  // 15. Stop while idle
  {
    worker.stop();
    const bool passed = worker.ready();

    printResult( "Stop while idle", passed );
    allPassed &= passed;
  }

  std::cout << "\nResult:     "
            << ( allPassed ? "PASS" : "FAIL" )
            << '\n';

  return allPassed ? 0 : 1;
}
