#include "tauron/stream/worker.hpp"

#include <stdexcept>

namespace tauron::stream {

Worker::Worker( WorkerId id, const crypto::RoundKeys& keys ) :
  id_( id ), keys_( keys ), state_( WorkerState::IDLE ),
  processed_( 0 ), written_( 0 ), time_( Clock::now() ) {}

WorkerResult Worker::run(
  Operation operation, std::span< const std::uint8_t > payload,
  std::span< std::uint8_t > output, bool eof
) {
  if ( state_ != WorkerState::IDLE )
    return { WorkerResultState::FAILED, 0, 0 };
}

void Worker::stop() {
  if ( state_ == WorkerState::PROCESSING )
    state_ = WorkerState::CANCELLED;
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
