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

  include/tauron/core/context.hpp
  include/tauron/core/state.hpp
  include/tauron/modules/module.hpp
  include/tauron/utils/hex.hpp
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
