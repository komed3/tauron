#include "tauron/stream/worker.hpp"

#include <stdexcept>

#include "tauron/core/constants.hpp"

namespace tauron::stream {

namespace {

constexpr std::size_t max_chunk_size( std::size_t blocks = core::SEQ_BLOCKS ) noexcept {
  return core::CHUNK_SEQS * core::BLOCK_SIZE * blocks;
}

constexpr std::size_t max_payload_size( std::size_t blocks = core::SEQ_BLOCKS ) noexcept {
  return core::CHUNK_SEQS * core::BLOCK_PAYLOAD * blocks;
}

} // namespace

Worker::Worker( WorkerId id, const crypto::RoundKeys& keys ) :
  id_( id ), keys_( keys ), state_( WorkerState::IDLE ),
  processed_( 0 ), written_( 0 ), time_( Clock::now() ) {}

WorkerResult Worker::run(
  Operation operation, std::span< const std::uint8_t > payload,
  std::span< std::uint8_t > output, bool eof
) {
  if ( state_ != WorkerState::IDLE )
    return { WorkerResultState::FAILED, 0, 0 };

  if ( payload.size() > max_payload_size() )
    throw std::invalid_argument( "Payload exceeds maximum chunk size" );

  if ( ! eof && payload.size() != max_payload_size() )
    throw std::invalid_argument( "Payload must be full size unless EOF is set" );

  reset_stats();
  state_ = WorkerState::PROCESSING;

  const auto size = operation == Operation::ENCRYPT
    ? encrypt( payload, output, eof )
    : decrypt( payload, output );

  const auto state = state_ == WorkerState::CANCELLED
    ? WorkerResultState::CANCELLED
    : WorkerResultState::COMPLETED;

  state_ = WorkerState::IDLE;
  return { state, payload.size(), size };
}

void Worker::stop() {
  if ( state_ == WorkerState::PROCESSING )
    state_ = WorkerState::CANCELLED;
}

std::size_t Worker::encrypt( std::span< const std::uint8_t > payload, std::span< std::uint8_t > output, bool eof ) {}

std::size_t Worker::decrypt( std::span< const std::uint8_t > payload, std::span< std::uint8_t > output ) {}

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
