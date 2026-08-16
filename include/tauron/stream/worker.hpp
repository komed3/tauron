#pragma once

#include <cstdint>

namespace tauron::stream {

using WorkerId = uint8_t;

class Worker {
public:
  explicit Worker( WorkerId id );
};

}
