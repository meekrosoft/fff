# Copyright 2022 Google LLC
# SPDX-License-Identifier: Apache-2.0

cmake_minimum_required(VERSION 3.20.0)
project(fff)

set(CMAKE_CXX_STANDARD 11)

# Enable ctest
enable_testing()

add_library(fff INTERFACE)

option(FFF_GENERATE "If enabled, fff.h will be regenerated using ruby" OFF)

# Generate fff.h if fakegen.rb changed
if(FFF_GENERATE)
  add_custom_command(
    OUTPUT
      ${CMAKE_CURRENT_LIST_DIR}/fff.h
    COMMAND
      ruby ${CMAKE_CURRENT_LIST_DIR}/fakegen.rb >> ${CMAKE_CURRENT_LIST_DIR}/fff.h
    DEPENDS
      ${CMAKE_CURRENT_LIST_DIR}/fakegen.rb
      ${CMAKE_CURRENT_LIST_DIR}/LICENSE
  )
  add_custom_target(fff_h DEPENDS ${CMAKE_CURRENT_LIST_DIR}/fff.h)
else()
  add_library(fff_h INTERFACE)
  set_target_properties(fff_h
    PROPERTIES PUBLIC_HEADER "fff.h"
  )
endif()

add_dependencies(fff fff_h)

# Add an interface library for fff.h
target_include_directories(fff INTERFACE ${CMAKE_CURRENT_LIST_DIR})

option(FFF_UNIT_TESTING "If enabled, fff tests will be compiled and run" OFF)

if(FFF_UNIT_TESTING)
  # Add tests and samples
  add_subdirectory(test)
  add_subdirectory(examples)
endif()
