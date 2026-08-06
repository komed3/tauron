#pragma once

#include "module.hpp"

namespace tauron::modules {

class EvolutionModule : public Module {
public:
  std::string_view name() const override;
  void process( core::Context& context ) override;
};

}
