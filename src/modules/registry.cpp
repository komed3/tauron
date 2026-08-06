#include "tauron/modules/registry.hpp"

namespace tauron::modules {

void Registry::add( const std::string& name, Factory factory ) {
  factories[ name ] = factory;
}

}
