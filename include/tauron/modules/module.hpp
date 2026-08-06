#pragma once

#include <string_view>

namespace tauron::core {

struct Context;

}

namespace tauron::modules {

class Module {
public:
  virtual ~Module() = default;
  virtual std::string_view name() const = 0;
  virtual void process( core::Context& context ) = 0;
};

}
