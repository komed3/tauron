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
  bool contains( const std::string& name ) const;
  std::size_t size() const;
  const Factory* find ( const std::string& name ) const;

  template< typename T >
  void add( const std::string& name ) {
    add( name, [] { return std::make_unique< T >() } );
  }

private:
  std::unordered_map< std::string, Factory > factories;
};

}
