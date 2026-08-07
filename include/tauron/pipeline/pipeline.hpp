#pragma once

#include <memory>
#include <vector>

#include "tauron/core/context.hpp"
#include "tauron/modules/module.hpp"

namespace tauron::pipeline {

class Pipeline {
public:
  void add( modules::ModuleType type, std::unique_ptr< modules::Module > module );
  void run( core::Context& context );

private:
  using ModuleList = std::vector< std::unique_ptr< modules::Module > >;
  ModuleList setup;
  ModuleList processing;
  ModuleList finalization;

  static void execute( Pipeline::ModuleList& modules, core::Context& context );
};

}
