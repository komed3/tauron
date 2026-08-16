#pragma once

#include <cstdint>

#include "tauron/crypto/keygen.hpp"

namespace tauron::stream {

using WorkerId = uint8_t;

enum class Operation {
  ENCRYPT,
  DECRYPT
};

enum class WorkerState {
  IDLE,
  PROCESSING
};

class Worker {
public:
  explicit Worker( WorkerId id, Operation operation, const crypto::RoundKeys& keys );
  void run();
  void stop();

private:
  WorkerId id;
  Operation operation;
  WorkerState state;
};

}
