#pragma once

#include <cstddef>

namespace tauron::crypto {

inline constexpr std::size_t BLOCK_SIZE = 32;
inline constexpr std::size_t KEY_SIZE = 32;

inline constexpr std::size_t MIN_ROUNDS = 2;
inline constexpr std::size_t MAX_ROUNDS = 128;

inline constexpr std::size_t WORDS = 8;

}
