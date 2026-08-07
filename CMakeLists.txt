cmake_minimum_required( VERSION 3.20 )

project(
  Tauron
  VERSION 0.1.0
  LANGUAGES CXX
)

set( CMAKE_CXX_STANDARD 20 )
set( CMAKE_CXX_STANDARD_REQUIRED ON )

add_library(
  tauron

  include/tauron/core/config.hpp
  include/tauron/core/context.hpp
  include/tauron/core/profile.hpp
  include/tauron/core/state.hpp

  include/tauron/modules/evolution.hpp
  include/tauron/modules/finalize.hpp
  include/tauron/modules/initialize.hpp
  include/tauron/modules/mix.hpp
  include/tauron/modules/module.hpp
  include/tauron/modules/registry.hpp
  include/tauron/modules/rotate.hpp
  include/tauron/modules/xor.hpp

  include/tauron/pipeline/definition.hpp
  include/tauron/pipeline/pipeline.hpp
  include/tauron/pipeline/step.hpp

  include/tauron/utils/hex.hpp


  src/modules/registry.cpp
)

target_include_directories(
  tauron
  PUBLIC
  include
)

add_executable(
  basic
  examples/basic.cpp
)

target_link_libraries(
  basic
  PRIVATE
  tauron
)

target_compile_options(
  tauron PRIVATE
    -Wall
    -Wextra
    -Wpedantic
)
