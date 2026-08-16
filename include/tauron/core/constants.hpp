#pragma once

#include <cstddef>
#include <cstdint>

namespace tauron::core {

inline constexpr std::size_t BLOCK_SIZE = 32;
inline constexpr std::size_t BLOCK_PAYLOAD = 28;

inline constexpr std::size_t WORDS = 8;

inline constexpr std::size_t KEY_SIZE = 32;
inline constexpr std::size_t NONCE_SIZE = 32;
inline constexpr std::size_t SALT_SIZE = 16;

inline constexpr std::size_t MIN_ROUNDS = 2;
inline constexpr std::size_t MAX_ROUNDS = 128;

inline constexpr std::size_t SEQ_BLOCKS = 256;
inline constexpr std::size_t CHUNK_SEQS = 128;

inline constexpr std::uint8_t KEY_PERMUT_DOMAIN = 0x01;
inline constexpr std::uint8_t SEQ_PERMUT_DOMAIN = 0x02;

}
