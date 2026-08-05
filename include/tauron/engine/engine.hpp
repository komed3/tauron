#pragma once

#include <memory>
#include <vector>

#include <tauron/core/module.hpp>

namespace tauron {
  class Engine {
  public:
    template< typename T, typename... Args >
    void use( Args&&... args );

    void process( Context& context );

  private:
    std::vector< std::unique_ptr< Module > > modules;
  };

  template< typename T, typename... Args >
  void Engine::use( Args&&... args ) {
    modules.push_back( std::make_unique< T >( std::forward< Args >( args )... ) );
  }
}
