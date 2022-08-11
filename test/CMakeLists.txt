# Copyright 2022 Google LLC
# SPDX-License-Identifier: Apache-2.0

# Create a list of common files needed for tests
set(
  COMMON_FILE_LIST
    include/c_test_framework.h
    src/test_cases.include
)

# Create the C test executable
add_executable(c_test src/fff_test_c.c ${COMMON_FILE_LIST})
target_include_directories(c_test PRIVATE include)
target_link_libraries(c_test PRIVATE fff)

# Create the C++ test executable
add_executable(cpp_test src/fff_test_cpp.cpp ${COMMON_FILE_LIST})
target_include_directories(cpp_test PRIVATE include)
target_link_libraries(cpp_test PRIVATE gtest fff)

# Create the C global test executable
add_executable(c_global_test
    src/fff_test_global_c.c
    src/global_fakes.c
    include/global_fakes.h
    ${COMMON_FILE_LIST}
)
target_include_directories(c_global_test PRIVATE include)
target_link_libraries(c_global_test PRIVATE fff)

# Create the C++ global test executable
add_executable(cpp_global_test
    src/fff_test_global_cpp.cpp
    src/global_fakes.c
    include/global_fakes.h
    ${COMMON_FILE_LIST}
)
target_include_directories(cpp_global_test PRIVATE include)
target_link_libraries(cpp_global_test PRIVATE gtest fff)

# Add the tests for ctest
add_test(
    NAME c_test
    COMMAND $<TARGET_FILE:c_test>
)

add_test(
    NAME cpp_test
    COMMAND $<TARGET_FILE:cpp_test>
)

add_test(
    NAME c_global_test
    COMMAND $<TARGET_FILE:c_global_test>
)

add_test(
    NAME cpp_global_test
    COMMAND $<TARGET_FILE:cpp_global_test>
)