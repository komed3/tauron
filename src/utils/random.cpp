#include "tauron/utils/random.hpp"

#include <memory>
#include <stdexcept>

#ifdef _WIN32

#include <windows.h>
#include <bcrypt.h>

#else

#include <sys/random.h>

#endif

namespace tauron::utils {

void Random::fill( std::span< std::uint8_t > buffer ) {
  if ( buffer.empty() ) return;

  #ifdef _WIN32

    std::size_t offset = 0;

    while ( offset < buffer.size() ) {
      const auto chunk = static_cast< ULONG >( std::min(
        buffer.size() - offset,
        static_cast< std::size_t >( ULONG_MAX )
      ) );

      const auto status = BCryptGenRandom(
        nullptr, buffer.data() + offset, chunk,
        BCRYPT_USE_SYSTEM_PREFERRED_RNG
      );

      if ( status != 0 )
        throw std::runtime_error( "Failed to generate cryptographically secure random data" );

      offset += chunk;
    }

  #else

    std::size_t offset = 0;

    while ( offset < buffer.size() ) {
      const auto result = getrandom( buffer.data() + offset, buffer.size() - offset, 0 );

      if ( result < 0 )
        throw std::runtime_error( "Failed to generate cryptographically secure random data" );

      if ( result == 0 )
        throw std::runtime_error( "Cryptographically secure random generator returned no data" );

      offset += static_cast< std::size_t >( result );
    }

  #endif
}

std::vector< std::uint8_t > Random::generate( std::size_t length ) {
  std::vector< std::uint8_t > result( length );
  if ( length == 0 ) return result;

  #ifdef _WIN32

    std::size_t offset = 0;

    while ( offset < length ) {
      const auto chunk = static_cast< ULONG >(
        std::min( length - offset, static_cast< std::size_t >( ULONG_MAX ) )
      );

      const auto status = BCryptGenRandom(
        nullptr, result.data() + offset, chunk,
        BCRYPT_USE_SYSTEM_PREFERRED_RNG
      );

      if ( status != 0 )
        throw std::runtime_error( "Failed to generate cryptographically secure random data" );

      offset += chunk;
    }

  #else

    std::size_t offset = 0;

    while ( offset < length ) {
      const auto generated = getrandom( result.data() + offset, length - offset, 0 );

      if ( generated < 0 )
        throw std::runtime_error( "Failed to generate cryptographically secure random data" );

      if ( generated == 0 )
        throw std::runtime_error( "Cryptographically secure random generator returned no data" );

      offset += static_cast< std::size_t >( generated );
    }

  #endif
}

Nonce Random::nonce() {
  Nonce result {};

  const auto data = generate( NONCE_SIZE );
  std::copy( data.begin(), data.end(), result.begin() );

  return result;
}

Salt Random::salt() {
  Salt result {};

  const auto data = generate( SALT_SIZE );
  std::copy( data.begin(), data.end(), result.begin() );

  return result;
}

}
