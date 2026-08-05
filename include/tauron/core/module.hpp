#pragma once

#include <string_view>

namespace tauron {
  class Context;

  class Module {
  public:
    virtual ~Module() = default;
    virtual std::string_view name() const = 0;
    virtual void process( Context& context ) = 0;
  };
}
