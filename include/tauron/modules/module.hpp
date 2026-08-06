#pragma once

#include "tauron/core/context.hpp"

namespace tauron::modules {

class Module {
public:
  virtual ~Module() = default;
  virtual void execute( core::Context& context ) = 0;
};

}
