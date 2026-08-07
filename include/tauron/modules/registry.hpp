#pragma once

#include <functional>
#include <memory>
#include <string>
#include <unordered_map>
#include <vector>

#include "tauron/modules/module.hpp"

namespace tauron::modules {

using ModuleFactory = std::function< std::unique_ptr< Module >() >;

class Registry {
public:
  bool add( ModuleFactory factory );

  template< typename T >
  bool add() { return add( [] { return std::make_unique< T >(); } ); }

  bool contains( const std::string& name ) const;
  std::size_t size() const;
  std::vector< std::string > names() const;
  const ModuleFactory* find ( const std::string& name ) const;

private:
  std::unordered_map< std::string, ModuleFactory > factories;
};

}
