#pragma once

#include <iostream>

#include <tauron/core/context.hpp>
#include <tauron/core/module.hpp>

namespace tauron {
  class RotationModule final : public Module {
  public:
    std::string_view name() const override {
      return "rotation";
    }
  };
}
