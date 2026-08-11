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

  include/tauron/stream/block.hpp
  include/tauron/stream/checksum.hpp


  src/crypto/block_cipher.cpp
  src/crypto/key_schedule.cpp
  src/crypto/master_key_generator.cpp
  src/crypto/nonce_generator.cpp

  src/stream/checksum.cpp
)

find_path( SODIUM_INCLUDE_DIR sodium.h REQUIRED )
find_library( SODIUM_LIBRARY sodium REQUIRED )

target_include_directories( tauron PRIVATE ${SODIUM_INCLUDE_DIR} )
target_link_libraries( tauron PRIVATE ${SODIUM_LIBRARY} )

if ( WIN32 )
  target_link_libraries( tauron PRIVATE bcrypt )
endif()

target_include_directories( tauron PUBLIC include )


add_executable( block_cipher_performance test/block_cipher_performance.cpp )
add_executable( block_cipher_test test/block_cipher_test.cpp )
add_executable( crypto_pipeline_test test/crypto_pipeline_test.cpp )
add_executable( key_schedule_test test/key_schedule_test.cpp )
add_executable( nonce_test test/nonce_test.cpp )
add_executable( passphrase_test test/passphrase_test.cpp )

target_link_libraries( block_cipher_performance PRIVATE tauron )
target_link_libraries( block_cipher_test PRIVATE tauron )
target_link_libraries( crypto_pipeline_test PRIVATE tauron )
target_link_libraries( key_schedule_test PRIVATE tauron )
target_link_libraries( nonce_test PRIVATE tauron )
target_link_libraries( passphrase_test PRIVATE tauron )


target_compile_options(
  tauron PRIVATE -Wall -Wextra -Wpedantic
  -std=c++20 -O3 -march=native -mtune=native -flto
)
