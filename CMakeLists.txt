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

  include/tauron/core/block.hpp
  include/tauron/core/constants.hpp
  include/tauron/crypto/cipher.hpp
  include/tauron/crypto/keygen.hpp
  include/tauron/utils/checksum.hpp
  include/tauron/utils/permutation.hpp
  include/tauron/utils/random.hpp
  include/tauron/utils/word.hpp

  src/core/block.cpp
  src/crypto/cipher.cpp
  src/crypto/keygen.cpp
  src/utils/checksum.cpp
  src/utils/permutation.cpp
  src/utils/random.cpp
)

find_path( SODIUM_INCLUDE_DIR sodium.h REQUIRED )
find_library( SODIUM_LIBRARY sodium REQUIRED )

target_include_directories( tauron PRIVATE ${SODIUM_INCLUDE_DIR} )
target_link_libraries( tauron PRIVATE ${SODIUM_LIBRARY} )

if ( WIN32 )
  target_link_libraries( tauron PRIVATE bcrypt )
endif()

target_include_directories( tauron PUBLIC include )


add_executable( block_test test/block_test.cpp )
add_executable( cipher_test test/cipher_test.cpp )
add_executable( keygen_test test/keygen_test.cpp )

target_link_libraries( block_test PRIVATE tauron )
target_link_libraries( cipher_test PRIVATE tauron )
target_link_libraries( keygen_test PRIVATE tauron )


target_compile_options(
  tauron PRIVATE -Wall -Wextra -Wpedantic
  -std=c++20 -O3 -march=native -mtune=native -flto
)
