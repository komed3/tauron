#include "tauron/stream/worker.hpp"

#include <stdexcept>

#include "tauron/core/constants.hpp"

namespace tauron::stream {

namespace {

inline constexpr std::size_t max_chunk_size() noexcept {
  return core::CHUNK_SEQS * core::SEQ_BLOCKS * core::BLOCK_SIZE;
}

inline constexpr std::size_t max_payload_size() noexcept {
  return core::CHUNK_SEQS * core::SEQ_BLOCKS * core::BLOCK_PAYLOAD;
}

std::size_t encrypt(
  std::span< const std::uint8_t > payload, std::span< std::uint8_t > output,
  bool eof, const crypto::RoundKeys& keys
);

std::size_t decrypt(
  std::span< const std::uint8_t > payload, std::span< std::uint8_t > output,
  const crypto::RoundKeys& keys
);

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
