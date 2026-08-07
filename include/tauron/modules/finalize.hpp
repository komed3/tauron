#pragma once

#include "module.hpp"

namespace tauron::modules {

class FinalizeModule : public Module {
public:
  const char* name() const override { return "finalize"; }
  ModuleType type() const override { return ModuleType::Finalization; }
  void execute( core::Context& context ) override {}
};

}
