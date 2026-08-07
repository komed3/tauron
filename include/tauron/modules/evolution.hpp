#pragma once

#include "tauron/modules/module.hpp"

namespace tauron::modules {

class EvolutionModule : public Module {
public:
  std::string_view name() const override { return "evolution"; }
  ModuleType type() const override { return ModuleType::Processing; }
  void execute( core::Context& context ) override;
};

}
