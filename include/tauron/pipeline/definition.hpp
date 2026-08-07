#pragma once

#include <string_view>
#include <cstddef>

#include "tauron/modules/module.hpp"

namespace tauron::pipeline {

struct Step {
  std::string_view module;
  modules::ModuleType type;
  std::size_t repeat = 1;
};

}
