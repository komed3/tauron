#pragma once

#include "module.hpp"

namespace tauron::modules {

class XorModule : public Module {
public:
  const char* name() const override { return "xor"; }
  ModuleType type() const override { return ModuleType::Processing; }
  void execute( core::Context& context ) override {}
};

}
