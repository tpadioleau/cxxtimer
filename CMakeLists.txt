cmake_minimum_required(VERSION 3.5)
project(cxxtimer VERSION 1.0)

set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

add_library(cxxtimer cxxtimer.cpp)
target_include_directories(cxxtimer PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})
target_compile_options(cxxtimer PRIVATE -Wall -Wextra -Wshadow -pedantic)

add_subdirectory(example)
