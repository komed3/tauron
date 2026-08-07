#pragma once

#include "tauron/modules/module.hpp"

namespace tauron::modules {

class FinalizeModule : public Module {
public:
  std::string_view name() const override { return "finalize"; }
  ModuleType type() const override { return ModuleType::Finalization; }
  void execute( core::Context& context ) override;
};

}
