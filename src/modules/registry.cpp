#include "tauron/modules/registry.hpp"

namespace tauron::modules {

void Registry::add( const std::string& name, Factory factory ) {
  factories[ name ] = factory;
}

std::unique_ptr< Module > Registry::create( const std::string& name ) const {
  auto iterator = factories.find( name );

  if ( iterator == factories.end() ) return nullptr;
  return iterator->second();
}

}
