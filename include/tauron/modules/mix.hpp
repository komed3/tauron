#pragma once

#include "module.hpp"

namespace tauron::modules {

class MixModule : public Module {
public:
  const char* name() const override { return "mix"; }
  void execute( core::Context& context ) override {}
};

}
