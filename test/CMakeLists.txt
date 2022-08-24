# Copyright 2022 Google LLC
# SPDX-License-Identifier: Apache-2.0

# Create a list of common files needed for tests
set(
  COMMON_FILE_LIST
    c_test_framework.h
    test_cases.include
)

# Create the C test executable
add_executable(c_test fff_test_c.c ${COMMON_FILE_LIST})
target_link_libraries(c_test PRIVATE fff)

# Create the C++ test executable
add_executable(cpp_test fff_test_cpp.cpp ${COMMON_FILE_LIST})
target_link_libraries(cpp_test PRIVATE gtest fff)

# Create the C global test executable
add_executable(c_global_test
    fff_test_global_c.c
    global_fakes.c
    global_fakes.h
    ${COMMON_FILE_LIST}
)
target_link_libraries(c_global_test PRIVATE fff)

# Create the C++ global test executable
add_executable(cpp_global_test
    fff_test_global_cpp.cpp
    global_fakes.c
    global_fakes.h
    ${COMMON_FILE_LIST}
)
target_link_libraries(cpp_global_test PRIVATE gtest fff)

# genertae fff_preamble.h so we can test --preamble command
add_custom_command(
  OUTPUT
    ${CMAKE_CURRENT_LIST_DIR}/fff_preamble.h
  COMMAND
    ruby ${CMAKE_CURRENT_LIST_DIR}/../fakegen.rb --preamble __wrap_ >> ${CMAKE_CURRENT_LIST_DIR}/fff_preamble.h
  DEPENDS
    ${CMAKE_CURRENT_LIST_DIR}/../fakegen.rb
    ${CMAKE_CURRENT_LIST_DIR}/../LICENSE
)
add_custom_target(fff_preamble_h DEPENDS ${CMAKE_CURRENT_LIST_DIR}/fff_preamble.h)

# Create the test executable to test fff_preamble.h
add_executable(c_preamble_test
    fff_preamble.h
    fff_test_preamble.c
    ${COMMON_FILE_LIST}
)

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

add_test(
    NAME c_preamble_test
    COMMAND $<TARGET_FILE:c_global_test>
)
