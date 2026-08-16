#include "tauron/stream/worker.hpp"

#include <stdexcept>

#include "tauron/core/constants.hpp"

namespace tauron::stream {

namespace {

inline constexpr std::size_t chunk_size() noexcept {
  return core::CHUNK_SEQS * core::SEQ_BLOCKS * core::BLOCK_PAYLOAD;
}

} // namespace

Worker::Worker( WorkerId id, Operation operation, const crypto::RoundKeys& keys ) :
  id_( id ), operation_( operation ), state_( WorkerState::IDLE ), keys( keys ),
  processed( 0 ), written( 0 ), time( Clock::now() ) {}

WorkerResult Worker::run( std::span< const std::uint8_t > payload, std::span< std::uint8_t > output, bool eof ) {
  if ( state_ != WorkerState::IDLE )
    return { WorkerResultState::FAILED, 0, 0 };

  if ( payload.size() > chunk_size() )
    throw std::invalid_argument( "Payload exceeds maximum chunk size" );

  if ( ! eof && payload.size() != chunk_size() )
    throw std::invalid_argument( "Payload must be full size unless EOF is set" );

  state_ = WorkerState::PROCESSING;
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
