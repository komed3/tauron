#include "tauron/crypto/nonce.hpp"

#include <stdexcept>

#ifdef _WIN32

#include <windows.h>
#include <bcrypt.h>

#pragma comment( lib, "bcrypt.lib" )

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
      throw std::runtime_error( "Faild to generate cryptographically secure nonce" );

  #else
    const auto result = getrandom( nonce.data(), nonce.size(), 0 );

    if ( result != static_cast< ssize_t >( nonce.size() ) )
      throw std::runtime_error( "Faild to generate cryptographically secure nonce" );

  #endif

  return nonce;
}

}
