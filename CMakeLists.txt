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
  include/tauron/crypto/constants.hpp
  include/tauron/crypto/key_schedule.hpp
  include/tauron/crypto/master_key_generator.hpp
  include/tauron/crypto/nonce_generator.hpp
  include/tauron/crypto/words.hpp

  src/crypto/block_cipher.cpp
  src/crypto/key_schedule.cpp
  src/crypto/nonce_generator.cpp
)

target_include_directories( tauron PUBLIC include )

add_executable( basic_key test/basic_key.cpp )
add_executable( bench_key test/bench_key.cpp )
add_executable( basic_block test/basic_block.cpp )
add_executable( bench_block test/bench_block.cpp )
add_executable( nonce_test test/nonce_test.cpp )

target_link_libraries( basic_key PRIVATE tauron )
target_link_libraries( bench_key PRIVATE tauron )
target_link_libraries( basic_block PRIVATE tauron )
target_link_libraries( bench_block PRIVATE tauron )
target_link_libraries( nonce_test PRIVATE tauron )

if ( WIN32 )
  target_link_libraries( nonce_test PRIVATE bcrypt )
endif()

target_compile_options(
  tauron PRIVATE -Wall -Wextra -Wpedantic
  -std=c++20 -O3 -march=native -mtune=native
)
