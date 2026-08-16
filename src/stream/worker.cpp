#include "tauron/stream/worker.hpp"

namespace tauron::stream {

namespace {



} // namespace

Worker::Worker( WorkerId id, Operation operation, const crypto::RoundKeys& keys ) :
  id( id ), operation( operation ), keys( keys ), state( WorkerState::IDLE ),
  processed( 0 ), written( 0 ), time_point( Clock::now() ) {}

}
