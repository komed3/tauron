#pragma once

namespace tauron::core {

enum class Profile {
  Fast,
  Balanced,
  Secure,
  Custom
};

struct CryptoConfig {};

struct StreamConfig {};

struct PerformanceConfig {};

struct DebugConfig {};

}