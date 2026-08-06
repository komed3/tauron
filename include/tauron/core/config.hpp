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

class Config {
public:
  Profile profile = Profile::Balanced;
  CryptoConfig crypto;
  StreamConfig stream;
  PerformanceConfig performance;
  DebugConfig debug;
};

}
