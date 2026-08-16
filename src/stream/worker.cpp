#include "tauron/stream/worker.hpp"

namespace tauron::stream {

namespace {



} // namespace

Worker::Worker( WorkerId id, Operation operation, const crypto::RoundKeys& keys ) :
  id_( id ), operation_( operation ), state_( WorkerState::IDLE ), keys( keys ),
  processed( 0 ), written( 0 ), time( Clock::now() ) {}

WorkerId Worker::id() const {
  return id_;
}

}
