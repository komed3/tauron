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

  include/tauron/constants.hpp
  include/tauron/core/block.hpp
  include/tauron/utils/checksum.hpp
  include/tauron/utils/random.hpp

  src/core/block.cpp
  src/utils/checksum.cpp
  src/utils/random.cpp
)
