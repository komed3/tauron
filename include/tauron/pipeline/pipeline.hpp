#pragma once

#include <memory>
#include <vector>

#include "tauron/core/context.hpp"
#include "tauron/modules/module.hpp"

namespace tauron::pipeline {

class Pipeline {
public:
  void add( std::unique_ptr< modules::Module > module );
  void process( core::Context& context );

private:
  std::vector< std::unique_ptr< modules::Module > > modules;
};

}
