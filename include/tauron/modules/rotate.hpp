#pragma once

#include "module.hpp"

namespace tauron::modules {

class RotateModule : public Module {
public:
  const char* name() const override { return "rotate"; }
  void execute( core::Context& context ) override {}
};

}
