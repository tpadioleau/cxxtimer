add_library(cxxtimer cxxtimer.cpp)
target_include_directories(cxxtimer PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})
target_compile_features(cxxtimer PUBLIC cxx_std_11)
target_compile_options(cxxtimer PRIVATE -Wall -Wextra -Wshadow -pedantic)
