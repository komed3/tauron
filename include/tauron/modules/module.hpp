#pragma once

namespace tauron::core {

struct Context;

}

namespace tauron::modules {

class Module {
public:
  virtual ~Module() = default;
  virtual void execute( core::Context& context ) = 0;
};

}
