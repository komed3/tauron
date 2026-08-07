#pragma once

#include "tauron/core/context.hpp"

namespace tauron::modules {

enum class ModuleType {
  Setup,
  Processing,
  Finalization
};

class Module {
public:
  virtual ~Module() = default;
  virtual const char* name() const = 0;
  virtual ModuleType type() const = 0;
  virtual void execute( core::Context& context ) = 0;
};

}
