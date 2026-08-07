#pragma once

#include "tauron/modules/module.hpp"

namespace tauron::modules {

class MixModule : public Module {
public:
  std::string_view name() const override { return "mix"; }
  ModuleType type() const override { return ModuleType::Processing; }
  void execute( core::Context& context ) override;
};

}
