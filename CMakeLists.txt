# Copyright 2022 Google LLC
# SPDX-License-Identifier: Apache-2.0

cmake_minimum_required(VERSION 3.20.0)
project(fff)

set(CMAKE_CXX_STANDARD 11)

# Enable ctest
enable_testing()

if(NOT DEFINED FFF_GENERATE)
  set(FFF_GENERATE TRUE)
endif()

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
endif()

add_custom_target(fff_h DEPENDS ${CMAKE_CURRENT_LIST_DIR}/fff.h)

# Add an interface library for fff.h
add_library(fff INTERFACE)
add_dependencies(fff fff_h)
target_include_directories(fff INTERFACE ${CMAKE_CURRENT_LIST_DIR})

if(NOT DEFINED FFF_UNIT_TESTING)
  set(FFF_UNIT_TESTING TRUE)
endif()

if(FFF_UNIT_TESTING)
  # Add tests and samples
  add_subdirectory(test)
  add_subdirectory(examples)
endif()
