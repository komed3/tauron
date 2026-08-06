#pragma once

#include "module.hpp"

namespace tauron::modules {

class CipherModule : public Module {
public:
  std::string_view name() const override;
  void process( core::Context& concept ) override;
};

}
