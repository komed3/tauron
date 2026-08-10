#include "tauron/crypto/nonce_generator.hpp"

#include <stdexcept>

#ifdef _WIN32

#include <windows.h>
#include <bcrypt.h>

#else

#include <sys/random.h>

#endif

namespace tauron::crypto {

Nonce NonceGenerator::generate() {
  Nonce nonce {};

  #ifdef _WIN32
    const auto status = BCryptGenRandom(
      nullptr, nonce.data(), static_cast< ULONG >( nonce.size() ),
      BCRYPT_USE_SYSTEM_PREFERRED_RNG
    );

    if ( status != 0 )
      throw std::runtime_error( "Failed to generate cryptographically secure nonce" );

  #else
    std::size_t offset = 0;

    while ( offset < nonce.size() )
      const auto result = getrandom( nonce.data() + offset, nonce.size() - offset, 0 );

    if ( result < 0 )
      throw std::runtime_error( "Failed to generate cryptographically secure nonce" );

    offset += static_cast< std::size_t >( result );

  #endif

  return nonce;
}

}
