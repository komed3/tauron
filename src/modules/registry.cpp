#include "tauron/modules/registry.hpp"

namespace tauron::modules {

void Registry::add( ModuleFactory factory ) {
  auto module = factory();
  factories.emplace( module->name(), std::move( factory ) );
}

bool Registry::contains( const std::string& name ) const {
  return factories.contains( name );
}

std::size_t Registry::size() const {
  return factories.size();
}

const ModuleFactory* Registry::find ( const std::string& name ) const {
  auto iterator = factories.find( name );

  if ( iterator == factories.end() ) return nullptr;
  return &iterator->second;
}

}
