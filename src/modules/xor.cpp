#include "tauron/modules/xor.hpp"

namespace tauron::modules {

void XorModule::execute( core::Context& context ) {
  for ( const auto& byte : context.buffers.input )
    context.buffers.output.push_back( byte ^ context.state.counter );
}

}