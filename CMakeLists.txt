# Copyright 2022 Google LLC
# SPDX-License-Identifier: Apache-2.0

cmake_minimum_required(VERSION 3.20.0)
project(fff)

set(CMAKE_CXX_STANDARD 11)

# Enable ctest
enable_testing()

option(FFF_GENERATE "If enabled, fff.h will be regenerated using ruby" OFF)

# Generate fff.h if fakegen.rb changed
if(FFF_GENERATE)
  find_package(Ruby REQUIRED)

  add_custom_command(
    OUTPUT
      ${CMAKE_BINARY_DIR}/fff.h
    COMMAND
      ${Ruby_EXECUTABLE} ${CMAKE_CURRENT_LIST_DIR}/fakegen.rb >> ${CMAKE_BINARY_DIR}/fff.h
    DEPENDS
      ${CMAKE_CURRENT_LIST_DIR}/fakegen.rb
      ${CMAKE_CURRENT_LIST_DIR}/LICENSE
  )
  add_library(fff INTERFACE ${CMAKE_BINARY_DIR}/fff.h)
  target_include_directories(fff INTERFACE
    $<BUILD_INTERFACE:${CMAKE_BINARY_DIR}>
    $<INSTALL_INTERFACE:${CMAKE_INSTALL_INCLUDEDIR}>
  )
  set_target_properties(fff
    PROPERTIES PUBLIC_HEADER "${CMAKE_BINARY_DIR}/fff.h"
  )
else()
  add_library(fff INTERFACE)
  target_include_directories(fff INTERFACE
    $<BUILD_INTERFACE:${CMAKE_SOURCE_DIR}>
    $<INSTALL_INTERFACE:${CMAKE_INSTALL_INCLUDEDIR}>
  )
  set_target_properties(fff
    PROPERTIES PUBLIC_HEADER "fff.h"
  )
endif()

option(FFF_UNIT_TESTING "If enabled, fff tests will be compiled and run" OFF)

if(FFF_UNIT_TESTING)
  # Add tests and samples
  add_subdirectory(test)
  add_subdirectory(examples)
endif()

include(GNUInstallDirs)

install(TARGETS fff EXPORT fffTargets
  PUBLIC_HEADER
  INCLUDES DESTINATION ${CMAKE_INSTALL_INCLUDEDIR}
)
install(EXPORT fffTargets
  FILE fffTargets.cmake
  NAMESPACE fff::
  DESTINATION ${CMAKE_INSTALL_DATAROOTDIR}/cmake/fff)
install(FILES fffConfig.cmake
  DESTINATION ${CMAKE_INSTALL_DATAROOTDIR}/cmake/fff)
