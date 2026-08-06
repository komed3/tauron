#pragma once

#include <functional>
#include <memory>
#include <string>
#include <unordered_map>

#include "module.hpp"

namespace tauron::modules {

class Registry {
public:
  using Factory = std::function< std::unique_ptr< Module >() >;
  void add( const std::string& name, Factory factory );
  std::unique_ptr< Module > create( const std::string& name ) const;

private:
  std::unordered_map< std::string, Factory > factories;
};

}
