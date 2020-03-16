add_library(cxxtimer STATIC cxxtimer.cpp)
target_include_directories(cxxtimer
  PUBLIC $<BUILD_INTERFACE:${PROJECT_SOURCE_DIR}/include>)
target_include_directories(cxxtimer
  SYSTEM PUBLIC $<INSTALL_INTERFACE:include>)
target_compile_features(cxxtimer PUBLIC cxx_std_11)
target_compile_options(cxxtimer PRIVATE -Wall -Wextra -Wshadow -pedantic)

################################################################################
# Installation procedure
################################################################################

# install target cxxtimer into cxxtimerTargets
install(
  TARGETS cxxtimer
  EXPORT cxxtimerTargets
  ARCHIVE DESTINATION lib/static)

# install cxxtimerTargets
export(
  EXPORT cxxtimerTargets
  FILE ${PROJECT_BINARY_DIR}/cxxtimerTargets.cmake
  NAMESPACE cxxtimer::)
