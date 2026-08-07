#include "tauron/modules/registry.hpp"

namespace tauron::modules {

bool Registry::add( ModuleFactory factory ) {
  auto module = factory();
  return factories.emplace( module->name(), std::move( factory ) ).second;
}

bool Registry::contains( const std::string& name ) const {
  return factories.contains( name );
}

std::size_t Registry::size() const {
  return factories.size();
}

std::vector< std::string > Registry::names() const {
  std::vector< std::string > result;
  result.reserve( this->size() );

  for ( const auto& [ name, _ ] : factories ) result.push_back( name );
  return result;
}

const ModuleFactory* Registry::find ( const std::string& name ) const {
  auto iterator = factories.find( name );

  if ( iterator == factories.end() ) return nullptr;
  return &iterator->second;
}

}
