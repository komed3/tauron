#pragma once

#include <memory>
#include <vector>

#include "tauron/core/context.hpp"
#include "tauron/modules/module.hpp"

namespace tauron::pipeline {

class Pipeline {
public:
  void add( modules::ModuleType type, std::unique_ptr< modules::Module > module );
  void execute( core::Context& context );

private:
  using ModuleList = std::vector< std::unique_ptr< modules::Module > >;

  ModuleList setup;
  ModuleList processing;
  ModuleList finalization;

  void execute( ModuleList& modules, core::Context context );
};

}
