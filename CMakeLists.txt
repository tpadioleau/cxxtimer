cmake_minimum_required(VERSION 3.5)

project(cxxtimer VERSION 1.0)

include(CTest)

set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

add_subdirectory(src)
add_subdirectory(example)

if (BUILD_TESTING)
  add_subdirectory(test)
endif (BUILD_TESTING)
