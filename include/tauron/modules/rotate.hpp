#pragma once

#include "module.hpp"

namespace tauron::modules {

class RotateModule : public Module {
public:
  std::string_view name() const override { return "rotate"; }
  ModuleType type() const override { return ModuleType::Processing; }
  void execute( core::Context& context ) override {}
};

}
