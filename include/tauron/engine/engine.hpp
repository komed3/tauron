#pragma once

#include <memory>
#include <vector>

#include <tauron/core/module.hpp>

namespace tauron {
  class Engine {
  public:
    void use( std::unique_ptr< Module > module );
    void process( Context& context );

  private:
    std::vector< std::unique_ptr< Module > > modules;
  };
}
