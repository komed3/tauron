#include "tauron/modules/registry.hpp"

namespace tauron::modules {

bool Registry::add( ModuleFactory factory ) {
  auto module = factory();
  return module && factories.emplace( module->name(), std::move( factory ) ).second;
}

bool Registry::contains( std::string_view name ) const {
  return factories.contains( std::string( name ) );
}

std::size_t Registry::size() const {
  return factories.size();
}

std::vector< std::string > Registry::names() const {
  std::vector< std::string > result;
  result.reserve( factories.size() );

  for ( const auto& [ name, _ ] : factories ) result.push_back( name );
  return result;
}

const ModuleFactory* Registry::find( std::string_view name ) const {
  auto iterator = factories.find( std::string( name ) );

  if ( iterator == factories.end() ) return nullptr;
  return &iterator->second;
}

}
