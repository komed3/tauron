#include <memory>

#include <tauron/core/context.hpp>
#include <tauron/engine/engine.hpp>
#include <tauron/modules/rotation_module.hpp>

int main() {
  tauron::Engine engine;
  tauron::Context context;

  engine.use( std::make_unique< tauron::RotationModule >() );
  engine.process( context );
}
