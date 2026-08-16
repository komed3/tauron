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
  explicit Worker( WorkerId id, Operation operation, const crypto::RoundKeys& keys );
  WorkerResult run( std::span< const std::uint8_t > payload, std::span< std::uint8_t > output, bool eof );
  void stop();

  WorkerId id() const;
  Operation operation() const;
  WorkerState state() const;

  std::size_t bytes_processed() const;
  std::size_t bytes_written() const;
  TimePoint last_progress() const;

private:
  WorkerId id_;
  Operation operation_;
  WorkerState state_;

  crypto::RoundKeys keys;

  std::size_t processed;
  std::size_t written;
  TimePoint time;
};

}
