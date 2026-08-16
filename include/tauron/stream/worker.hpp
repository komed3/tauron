#pragma once

#include <chrono>
#include <cstddef>
#include <cstdint>
#include <span>

#include "tauron/crypto/keygen.hpp"

namespace tauron::stream {

using WorkerId = std::uint8_t;
using Clock = std::chrono::steady_clock;
using TimePoint = Clock::time_point;

enum class Operation {
  ENCRYPT,
  DECRYPT
};

enum class WorkerState {
  IDLE,
  PROCESSING,
  CANCELLED
};

enum class WorkerResultState {
  COMPLETED,
  CANCELLED,
  FAILED
};

struct WorkerResult {
  WorkerResultState state;
  std::size_t bytes_read;
  std::size_t bytes_written;
};

class Worker {
public:
  explicit Worker( WorkerId id, const crypto::RoundKeys& keys );
  WorkerResult run( Operation operation, std::span< const std::uint8_t > payload, std::span< std::uint8_t > output, bool eof );
  void stop();

  WorkerId id() const;
  WorkerState state() const;

  std::size_t bytes_processed() const;
  std::size_t bytes_written() const;
  TimePoint last_progress() const;

private:
  std::size_t encrypt( std::span< const std::uint8_t > payload, std::span< std::uint8_t > output, bool eof );
  std::size_t decrypt( std::span< const std::uint8_t > payload, std::span< std::uint8_t > output );

  void reset_stats();
  void update_progress( std::size_t processed, std::size_t written );

  WorkerId id_;
  crypto::RoundKeys keys_;
  WorkerState state_;

  std::size_t processed_;
  std::size_t written_;
  TimePoint time_;
};

}
