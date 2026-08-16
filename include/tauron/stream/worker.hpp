#pragma once

#include <cstdint>

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
  explicit Worker( WorkerId id, Operation operation );

private:
  WorkerId id;
  Operation operation;
  WorkerState state;
};

}
