#include "tauron/stream/worker.hpp"

#include <algorithm>
#include <stdexcept>

#include "tauron/core/constants.hpp"
#include "tauron/core/sequence.hpp"
#include "tauron/crypto/cipher.hpp"

namespace tauron::stream {

namespace {} // namespace

Worker::Worker( WorkerId id, const crypto::RoundKeys& keys ) :
  id_( id ), keys_( keys ), state_( WorkerState::IDLE ),
  processed_( 0 ), written_( 0 ), time_( Clock::now() ) {}

WorkerResult Worker::run(
  Operation operation, std::span< const std::uint8_t > payload,
  std::span< std::uint8_t > output, bool eof
) {
  if ( state_ != WorkerState::IDLE )
    return { WorkerResultState::FAILED, 0, 0 };

  reset_stats();
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

void Worker::stop() {
  if ( state_ == WorkerState::PROCESSING )
    state_ = WorkerState::CANCELLED;
}

std::size_t Worker::encrypt( std::span< const std::uint8_t > payload, std::span< std::uint8_t > output, bool eof ) {
  if ( payload.empty() ) return 0;

  if ( payload.size() > core::CHUNK_SEQS * core::SEQ_BLOCKS * core::BLOCK_PAYLOAD )
    throw std::invalid_argument( "Payload exceeds maximum chunk size" );

  if ( ! eof && payload.size() != core::CHUNK_SEQS * core::SEQ_BLOCKS * core::BLOCK_PAYLOAD )
    throw std::invalid_argument( "Payload must be full size unless EOF is set" );

  const auto sequence_size = core::SEQ_BLOCKS * core::BLOCK_PAYLOAD;
  const auto count = ( payload.size() + sequence_size - 1 ) / sequence_size;

  if ( output.size() < count * core::SEQ_BLOCKS * core::BLOCK_SIZE )
    throw std::invalid_argument( "Output buffer is too small" );

  std::size_t processed = 0;
  std::size_t written = 0;

  for ( std::size_t i = 0; i < count; ++i ) {
    const auto size = std::min( sequence_size, payload.size() - processed );
    const auto sequence = core::Sequence::build( payload.subspan( processed, size ), eof && i + 1 == count );

    for ( std::size_t j = 0; j < sequence.count; ++j ) {
      const auto block = crypto::Cipher::encrypt( sequence.blocks[ j ], keys_ );

      std::copy( block.begin(), block.end(), output.begin() + written );
      written += block.size();
    }

    processed += size;
    update_progress( processed, written );

    if ( state_ == WorkerState::CANCELLED )
      return written;
  }

  return written;
}

std::size_t Worker::decrypt( std::span< const std::uint8_t > payload, std::span< std::uint8_t > output ) {
  if ( payload.empty() ) return 0;

  if ( payload.size() > core::CHUNK_SEQS * core::SEQ_BLOCKS * core::BLOCK_SIZE )
    throw std::invalid_argument( "Payload exceeds maximum chunk size" );

  if ( payload.size() % core::BLOCK_SIZE != 0 )
    throw std::invalid_argument( "Payload size is not aligned to block size" );

  const auto sequence_size = core::SEQ_BLOCKS * core::BLOCK_SIZE;
  const auto count = ( payload.size() + sequence_size - 1 ) / sequence_size;

  if ( output.size() < count * core::SEQ_BLOCKS * core::BLOCK_PAYLOAD )
    throw std::invalid_argument( "Output buffer is too small" );
}

void Worker::reset_stats() {
  processed_ = 0;
  written_ = 0;
  time_ = Clock::now();
}

void Worker::update_progress( std::size_t processed, std::size_t written ) {
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

std::size_t Worker::bytes_processed() const {
  return processed_;
}

std::size_t Worker::bytes_written() const {
  return written_;
}

TimePoint Worker::last_progress() const {
  return time_;
}

}
