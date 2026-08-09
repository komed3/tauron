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

  include/tauron/crypto/block_cipher.hpp
  include/tauron/crypto/key_schedule.hpp

  src/crypto/key_schedule.cpp
)

target_include_directories( tauron PUBLIC include )

add_executable( basic_key test/basic_key.cpp )
add_executable( bench_key test/bench_key.cpp )

target_link_libraries( basic_key PRIVATE tauron )
target_link_libraries( bench_key PRIVATE tauron )

target_compile_options( tauron PRIVATE -Wall -Wextra -Wpedantic )
