#include "tauron/crypto/nonce.hpp"

#include <stdexcept>

#ifdef _WIN32

#include <windows.h>
#include <bcrypt.h>

#pragma comment( lib, "bcrypt.lib" )

#else

#include <sys/random.h>

#endif
