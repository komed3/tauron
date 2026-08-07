#pragma once

#include <string_view>

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
  virtual [[nodiscard]] std::string_view name() const = 0;
  virtual [[nodiscard]] ModuleType type() const = 0;
  virtual void execute( core::Context& context ) = 0;
};

}
