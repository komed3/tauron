#include <memory>

#include <tauron/core/context.hpp>
#include <tauron/engine/engine.hpp>
#include <tauron/modules/rotation_module.hpp>

int main() {
  tauron::Engine engine;
  tauron::Context context;

  engine.use< tauron::RotationModule >();
  engine.process( context );
}
