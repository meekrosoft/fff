# Copyright 2022 Google LLC
# SPDX-License-Identifier: Apache-2.0

cmake_minimum_required(VERSION 3.20.0)

find_package(Threads REQUIRED)
set(CMAKE_EXE_LINKER_FLAGS " -static")

# Create the gtest library
add_library(gtest src/gtest-all.cc src/gtest-main.cc)
target_include_directories(gtest PUBLIC include include/gtest)
target_link_libraries(gtest PRIVATE Threads::Threads -static-libstdc++)
