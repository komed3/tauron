#pragma once

#include <cstdint>
#include <span>

#include "tauron/crypto/keygen.hpp"

namespace tauron::stream {

using WorkerId = std::uint8_t;

enum class Operation {
  ENCRYPT,
  DECRYPT
};

enum class WorkerState {
  IDLE,
  PROCESSING
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

private:
  WorkerId id;
  Operation operation;
  WorkerState state;
};

}
