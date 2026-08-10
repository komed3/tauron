#include <chrono>
#include <cstdint>
#include <iomanip>
#include <iostream>
#include <vector>

#include "tauron/crypto/block_cipher.hpp"
#include "tauron/crypto/key_schedule.hpp"
#include "tauron/crypto/nonce_generator.hpp"

using namespace tauron::crypto;
using Clock = std::chrono::steady_clock;

constexpr std::size_t BLOCKS = 1'000'000;
constexpr std::size_t ROUNDS = 16;

int main() {}
