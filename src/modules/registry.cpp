#include "tauron/modules/registry.hpp"

namespace tauron::modules {

void Registry::add( const std::string& name, ModuleFactory factory ) {
  factories[ name ] = std::move( factory );
}

bool Registry::contains( const std::string& name ) const {
  return factories.contains( name );
}

std::size_t Registry::size() const {
  return factories.size();
}

const Registry::ModuleFactory* Registry::find ( const std::string& name ) const {
  auto iterator = factories.find( name );

  if ( iterator == factories.end() ) return nullptr;
  return &iterator->second;
}

}
