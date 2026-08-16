#include "tauron/stream/worker.hpp"

#include <algorithm>
#include <array>
#include <stdexcept>

#include "tauron/core/constants.hpp"
#include "tauron/core/sequence.hpp"
#include "tauron/crypto/cipher.hpp"

namespace tauron::stream {

namespace {

constexpr std::size_t SEQ_PAYLOAD_SIZE =    core::SEQ_BLOCKS * core::BLOCK_PAYLOAD;
constexpr std::size_t SEQ_CIPHERTEXT_SIZE = core::SEQ_BLOCKS * core::BLOCK_SIZE;
constexpr std::size_t MAX_PAYLOAD_SIZE =    core::CHUNK_SEQS * SEQ_PAYLOAD_SIZE;
constexpr std::size_t MAX_CIPHERTEXT_SIZE = core::CHUNK_SEQS * SEQ_CIPHERTEXT_SIZE;

constexpr std::memory_order ORDER = std::memory_order::relaxed;

Clock::duration::rep now() noexcept {
  return Clock::now().time_since_epoch().count();
}

} // namespace

Worker::Worker( WorkerId id, const crypto::RoundKeys& keys ) :
  id_( id ), keys_( keys ), stop_requested_( false ), state_( WorkerState::IDLE ),
  processed_( 0 ), written_( 0 ), activity_( now() ) {}

void Worker::stop() {
  if ( state_.load( ORDER ) == WorkerState::PROCESSING )
    stop_requested_.store( true, ORDER );
}

WorkerResult Worker::run(
  Operation operation, std::span< const std::uint8_t > payload,
  std::span< std::uint8_t > output, bool eof
) {
  if ( state_.load( ORDER ) != WorkerState::IDLE )
    return { WorkerResultState::FAILED, 0, 0 };

  resetStats();
  stop_requested_.store( false, ORDER );
  state_.store( WorkerState::PROCESSING, ORDER );

  try {
    const auto size = operation == Operation::ENCRYPT
      ? encrypt( payload, output, eof )
      : decrypt( payload, output );

    const auto state = state_.load( ORDER ) == WorkerState::CANCELLED
      ? WorkerResultState::CANCELLED
      : WorkerResultState::COMPLETED;

    state_.store( WorkerState::IDLE, ORDER );
    stop_requested_.store( false, ORDER );

    return { state, payload.size(), size };
  } catch ( ... ) {
    state_.store( WorkerState::IDLE, ORDER );
    stop_requested_.store( false, ORDER );

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

    if ( stop_requested_.load( ORDER ) ) {
      state_.store( WorkerState::CANCELLED, ORDER );
      return written;
    }
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

    if ( stop_requested_.load( ORDER ) ) {
      state_.store( WorkerState::CANCELLED, ORDER );
      return written;
    }
  }

  return written;
}

void Worker::resetStats() {
  processed_.store( 0, ORDER );
  written_.store( 0, ORDER );
  activity_.store( now(), ORDER );
}

void Worker::updateProgress( std::size_t processed, std::size_t written ) {
  processed_.store( processed, ORDER );
  written_.store( written, ORDER );
  activity_.store( now(), ORDER );
}

WorkerId Worker::id() const {
  return id_;
}

WorkerState Worker::state() const {
  return state_.load( ORDER );
}

bool Worker::ready() const {
  return state_.load( ORDER ) == WorkerState::IDLE;
}

std::size_t Worker::processed() const {
  return processed_.load( ORDER );
}

std::size_t Worker::written() const {
  return written_.load( ORDER );
}

TimePoint Worker::activity() const {
  return TimePoint( Clock::duration( activity_.load( ORDER ) ) );
}

}
