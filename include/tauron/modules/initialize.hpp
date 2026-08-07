#pragma once

#include "module.hpp"

namespace tauron::modules {

class InitializeModule : public Module {
public:
  std::string_view name() const override { return "initialize"; }
  ModuleType type() const override { return ModuleType::Setup; }
  void execute( core::Context& context ) override {}
};

}
