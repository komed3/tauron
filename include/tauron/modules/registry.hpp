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

  [[nodiscard]] bool contains( std::string_view name ) const;
  [[nodiscard]] std::size_t size() const;
  [[nodiscard]] std::vector< std::string > names() const;
  [[nodiscard]] const ModuleFactory* find( std::string_view name ) const;

private:
  std::unordered_map< std::string, ModuleFactory > factories;
};

}
