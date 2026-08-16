#include "tauron/stream/worker.hpp"

#include <algorithm>
#include <array>
#include <stdexcept>

#include "tauron/core/constants.hpp"
#include "tauron/core/sequence.hpp"
#include "tauron/crypto/cipher.hpp"

namespace tauron::stream {

namespace {

inline constexpr std::size_t SEQ_PAYLOAD_SIZE =    core::SEQ_BLOCKS * core::BLOCK_PAYLOAD;
inline constexpr std::size_t SEQ_CIPHERTEXT_SIZE = core::SEQ_BLOCKS * core::BLOCK_SIZE;
inline constexpr std::size_t MAX_PAYLOAD_SIZE =    core::CHUNK_SEQS * SEQ_PAYLOAD_SIZE;
inline constexpr std::size_t MAX_CIPHERTEXT_SIZE = core::CHUNK_SEQS * SEQ_CIPHERTEXT_SIZE;

} // namespace

Worker::Worker( WorkerId id, const crypto::RoundKeys& keys ) :
  id_( id ), keys_( keys ), state_( WorkerState::IDLE ),
  processed_( 0 ), written_( 0 ), time_( Clock::now() ) {}

void Worker::stop() {
  if ( state_ == WorkerState::PROCESSING )
    state_ = WorkerState::CANCELLED;
}

WorkerResult Worker::run(
  Operation operation, std::span< const std::uint8_t > payload,
  std::span< std::uint8_t > output, bool eof
) {
  if ( state_ != WorkerState::IDLE )
    return { WorkerResultState::FAILED, 0, 0 };

  resetStats();
  state_ = WorkerState::PROCESSING;

  try {
    const auto size = operation == Operation::ENCRYPT
      ? encrypt( payload, output, eof )
      : decrypt( payload, output );

    const auto state = state_ == WorkerState::CANCELLED
      ? WorkerResultState::CANCELLED
      : WorkerResultState::COMPLETED;

    state_ = WorkerState::IDLE;
    return { state, payload.size(), size };
  } catch ( ... ) {
    state_ = WorkerState::IDLE;
    throw;
  }
}

std::size_t Worker::encrypt( std::span< const std::uint8_t > payload, std::span< std::uint8_t > output, bool eof ) {
  if ( payload.empty() ) return 0;

  if ( payload.size() > MAX_PAYLOAD_SIZE )
    throw std::invalid_argument( "Payload exceeds maximum chunk size" );

  if ( ! eof && payload.size() != MAX_PAYLOAD_SIZE )
    throw std::invalid_argument( "Payload must be full size unless EOF is set" );

  const auto count = ( payload.size() + SEQ_PAYLOAD_SIZE - 1 ) / SEQ_PAYLOAD_SIZE;

  if ( output.size() < count * SEQ_CIPHERTEXT_SIZE )
    throw std::invalid_argument( "Output buffer is too small" );

  std::size_t processed = 0;
  std::size_t written = 0;

  for ( std::size_t i = 0; i < count; ++i ) {
    const auto size = std::min( SEQ_PAYLOAD_SIZE, payload.size() - processed );
    const auto sequence = core::Sequence::build( payload.subspan( processed, size ), eof && i + 1 == count );

    for ( std::size_t j = 0; j < sequence.count; ++j ) {
      const auto block = crypto::Cipher::encrypt( sequence.blocks[ j ], keys_ );

      std::copy( block.begin(), block.end(), output.begin() + written );
      written += block.size();
    }

    processed += size;
    updateProgress( processed, written );

    if ( state_ == WorkerState::CANCELLED )
      return written;
  }

  return written;
}

std::size_t Worker::decrypt( std::span< const std::uint8_t > payload, std::span< std::uint8_t > output ) {
  if ( payload.empty() ) return 0;

  if ( payload.size() > MAX_CIPHERTEXT_SIZE )
    throw std::invalid_argument( "Payload exceeds maximum chunk size" );

  if ( payload.size() % core::BLOCK_SIZE != 0 )
    throw std::invalid_argument( "Payload size is not aligned to block size" );

  const auto count = ( payload.size() + SEQ_CIPHERTEXT_SIZE - 1 ) / SEQ_CIPHERTEXT_SIZE;

  if ( output.size() < count * SEQ_PAYLOAD_SIZE )
    throw std::invalid_argument( "Output buffer is too small" );

  std::array< core::DataBlock, core::SEQ_BLOCKS > blocks {};
  std::size_t processed = 0;
  std::size_t written = 0;

  for ( std::size_t i = 0; i < count; ++i ) {
    const auto size = std::min( SEQ_CIPHERTEXT_SIZE, payload.size() - processed );
    const auto block_count = size / core::BLOCK_SIZE;

    for ( std::size_t j = 0; j < block_count; ++j ) {
      std::copy(
        payload.begin() + processed + j * core::BLOCK_SIZE,
        payload.begin() + processed + ( j + 1 ) * core::BLOCK_SIZE,
        blocks[ j ].begin()
      );

      blocks[ j ] = crypto::Cipher::decrypt( blocks[ j ], keys_ );
    }

    const auto parsed = core::Sequence::parse(
      std::span< const core::DataBlock >( blocks.data(), block_count ),
      output.subspan( written )
    );

    processed += size;
    written += parsed;
    updateProgress( processed, written );

    if ( state_ == WorkerState::CANCELLED )
      return written;
  }

  return written;
}

void Worker::resetStats() {
  processed_ = 0;
  written_ = 0;
  time_ = Clock::now();
}

void Worker::updateProgress( std::size_t processed, std::size_t written ) {
  processed_ = processed;
  written_ = written;
  time_ = Clock::now();
}

WorkerId Worker::id() const {
  return id_;
}

WorkerState Worker::state() const {
  return state_;
}

std::size_t Worker::bytesProcessed() const {
  return processed_;
}

std::size_t Worker::bytesWritten() const {
  return written_;
}

TimePoint Worker::lastProgress() const {
  return time_;
}

}
