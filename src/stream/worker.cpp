#include "tauron/stream/worker.hpp"

#include <stdexcept>

#include "tauron/core/constants.hpp"
#include "tauron/core/sequence.hpp"
#include "tauron/crypto/cipher.hpp"

namespace tauron::stream {

namespace {

inline constexpr std::size_t max_chunk_size() noexcept {
  return core::CHUNK_SEQS * core::SEQ_BLOCKS * core::BLOCK_SIZE;
}

inline constexpr std::size_t max_payload_size() noexcept {
  return core::CHUNK_SEQS * core::SEQ_BLOCKS * core::BLOCK_PAYLOAD;
}

inline constexpr std::size_t max_seq_size() noexcept {
  return core::SEQ_BLOCKS * core::BLOCK_SIZE;
}

inline constexpr std::size_t seq_payload_size() noexcept {
  return core::SEQ_BLOCKS * core::BLOCK_PAYLOAD;
}

std::size_t encrypt(
  std::span< const std::uint8_t > payload, std::span< std::uint8_t > output,
  bool eof, const crypto::RoundKeys& keys
) {
  const auto sequence_size = seq_payload_size();
  const auto count = ( payload.size() + sequence_size - 1 ) / sequence_size;
  std::size_t written = 0;

  for ( std::size_t i = 0; i < count; ++i ) {
    const auto offset = i * sequence_size;
    const auto size = std::min( sequence_size, payload.size() - offset );
    const bool last = i + 1 == count;

    const auto sequence = core::Sequence::build( payload.subspan( offset, size ), eof && last );

    for ( std::size_t j = 0; j < sequence.count; ++j ) {
      const auto block = crypto::Cipher::encrypt( sequence.blocks[ j ], keys );

      std::copy( block.begin(), block.end(), output.begin() + written );
      written += block.size();
    }
  }

  return written;
}

std::size_t decrypt(
  std::span< const std::uint8_t > payload, std::span< std::uint8_t > output,
  const crypto::RoundKeys& keys
) {
  const auto sequence_size = max_seq_size();
  const auto count = payload.size() / sequence_size;
  std::size_t written = 0;
}

} // namespace

Worker::Worker( WorkerId id, Operation operation, const crypto::RoundKeys& keys ) :
  id_( id ), operation_( operation ), state_( WorkerState::IDLE ), keys( keys ),
  processed( 0 ), written( 0 ), time( Clock::now() ) {}

WorkerResult Worker::run( std::span< const std::uint8_t > payload, std::span< std::uint8_t > output, bool eof ) {
  if ( state_ != WorkerState::IDLE )
    return { WorkerResultState::FAILED, 0, 0 };

  if ( payload.size() > max_payload_size() )
    throw std::invalid_argument( "Payload exceeds maximum chunk size" );

  if ( ! eof && payload.size() != max_payload_size() )
    throw std::invalid_argument( "Payload must be full size unless EOF is set" );

  if ( output.size() < max_chunk_size() )
    throw std::invalid_argument( "Output buffer is too small" );

  state_ = WorkerState::PROCESSING;

  const auto size = operation_ == Operation::ENCRYPT
    ? encrypt( payload, output, eof, keys )
    : decrypt( payload, output, keys );

  processed = payload.size();
  written = size;
  time = Clock::now();

  const auto state = state_ == WorkerState::CANCELLED
    ? WorkerResultState::CANCELLED
    : WorkerResultState::COMPLETED;

  state_ = WorkerState::IDLE;
  return { state, payload.size(), size };
}

void Worker::stop() {
  state_ = WorkerState::CANCELLED;
}

WorkerId Worker::id() const {
  return id_;
}

Operation Worker::operation() const {
  return operation_;
}

WorkerState Worker::state() const {
  return state_;
}

std::size_t Worker::bytes_processed() const {
  return processed;
}

std::size_t Worker::bytes_written() const {
  return written;
}

TimePoint Worker::last_progress() const {
  return time;
}

}
