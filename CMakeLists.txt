# Copyright 2022 Google LLC
# SPDX-License-Identifier: Apache-2.0

cmake_minimum_required(VERSION 3.20.0)
project(fff)

set(CMAKE_CXX_STANDARD 11)

# Enable easier enabling/disabling of example and test builds
option(FFF_BUILD_TEST "If set to ON, unit tests will be compiled" ON)
option(BuildExample "If set to ON, examples will be compiled" ON)

if(FFF_BUILD_TEST)
  # Add the gtest library which will be used below
  add_subdirectory(gtest)

  # Enable ctest
  enable_testing()
endif()

# Generate fff.h if fakegen.rb changed
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

# Add an interface library for fff.h
add_library(fff INTERFACE)
add_dependencies(fff fff_h)
target_include_directories(fff INTERFACE ${CMAKE_CURRENT_LIST_DIR})

# Add tests and samples
if(FFF_BUILD_TEST)
  add_subdirectory(test)
endif()
if(BuildExample)
  add_subdirectory(examples)
endif()
