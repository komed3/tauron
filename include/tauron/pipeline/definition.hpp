#pragma once

#include <array>

#include "tauron/core/profile.hpp"
#include "tauron/pipeline/step.hpp"

namespace tauron::pipeline {

inline constexpr std::array defaultProfile {
  Step { "initialize", modules::ModuleType::Setup },
  Step { "evolution", modules::ModuleType::Processing },
  Step { "mix", modules::ModuleType::Processing },
  Step { "rotate", modules::ModuleType::Processing },
  Step { "xor", modules::ModuleType::Processing },
  Step { "finalize", modules::ModuleType::Finalization }
};

inline constexpr std::array fastProfile {
  Step { "initialize", modules::ModuleType::Setup },
  Step { "evolution", modules::ModuleType::Processing },
  Step { "xor", modules::ModuleType::Processing },
  Step { "finalize", modules::ModuleType::Finalization }
};

inline constexpr std::array experimentalProfile {
  Step { "initialize", modules::ModuleType::Setup },
  Step { "evolution", modules::ModuleType::Processing, 3 },
  Step { "mix", modules::ModuleType::Processing, 2 },
  Step { "rotate", modules::ModuleType::Processing },
  Step { "xor", modules::ModuleType::Processing },
  Step { "finalize", modules::ModuleType::Finalization }
};

}
