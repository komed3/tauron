#include "tauron/core/engine.hpp"
#include "tauron/modules/evolution.hpp"
#include "tauron/modules/finalize.hpp"
#include "tauron/modules/initialize.hpp"
#include "tauron/modules/mix.hpp"
#include "tauron/modules/rotate.hpp"
#include "tauron/modules/xor.hpp"
#include "tauron/pipeline/builder.hpp"

namespace tauron::core {

void Engine::initialize() {
  registry.add< modules::InitializeModule >();
  registry.add< modules::EvolutionModule >();
  registry.add< modules::MixModule >();
  registry.add< modules::RotateModule >();
  registry.add< modules::XorModule >();
  registry.add< modules::FinalizeModule >();

  pipeline = pipeline::Builder::build( config.profile, registry );
}

std::vector< std::uint8_t > Engine::process( std::string_view input, Operation operation ) {
  Context context;
  context.config = config;
  context.operation = operation;

  context.buffers.input.assign( input.begin(), input.end() );
  pipeline.run( context );

  return std::move( context.buffers.output );
}

}
