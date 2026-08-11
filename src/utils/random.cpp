#include "tauron/utils/random.hpp"

#include <stdexcept>

#ifdef _WIN32

#include <windows.h>
#include <bcrypt.h>

#else

#include <sys/random.h>

#endif

namespace tauron::utils {

std::span< std::uint8_t > Random::generate( std::size_t length ) {}

Nonce Random::nonce() {}

Salt Random::salt() {}

}
