cmake_minimum_required(VERSION 3.9)

project(cxxtimer VERSION 1.0 LANGUAGES CXX)

include(CTest)

add_subdirectory(src)
add_subdirectory(example)

if (BUILD_TESTING) # defined by CTest
  add_subdirectory(test)
endif (BUILD_TESTING)
