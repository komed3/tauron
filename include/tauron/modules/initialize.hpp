#pragma once

#include "module.hpp"

namespace tauron::modules {

class InitializeModule : public Module {
public:
  const char* name() const override { return "initialize"; }
  ModuleType type() const override { return ModuleType::Setup; }
  void execute( core::Context& context ) override {}
};

}
