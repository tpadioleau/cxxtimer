cmake_minimum_required(VERSION 3.9)

project(cxxtimer VERSION 1.0 LANGUAGES CXX)

add_subdirectory(src)
add_subdirectory(example)

################################################################################
# Testing procedure
################################################################################

include(CTest)
if (BUILD_TESTING) # defined by CTest
  add_subdirectory(test)
endif (BUILD_TESTING)

################################################################################
# Installation procedure
################################################################################

# install headers
install(
  DIRECTORY ${PROJECT_SOURCE_DIR}/include/
  DESTINATION include)

include(CMakePackageConfigHelpers)

# generate the config file
configure_package_config_file(
  cmake/cxxtimerConfig.cmake.in
  ${PROJECT_BINARY_DIR}/cxxtimerConfig.cmake
  INSTALL_DESTINATION cmake/cxxtimer)

# generate the version file for the config file
write_basic_package_version_file(
  ${PROJECT_BINARY_DIR}/cxxtimerConfigVersion.cmake
  COMPATIBILITY SameMajorVersion)

install(
  FILES ${PROJECT_BINARY_DIR}/cxxtimerTargets.cmake
        ${PROJECT_BINARY_DIR}/cxxtimerConfig.cmake
        ${PROJECT_BINARY_DIR}/cxxtimerConfigVersion.cmake
  DESTINATION cmake/cxxtimer)
