# Copyright 2022 Google LLC
# SPDX-License-Identifier: Apache-2.0

# Create a list of common files needed for tests
set(
  COMMON_FILE_LIST
    include/c_test_framework.h
    src/test_cases.include
)

include(FetchContent)

FetchContent_Declare(
  googletest
  GIT_REPOSITORY https://github.com/google/googletest.git
  GIT_TAG v1.12.0
)

set(gtest_force_shared_crt ON CACHE BOOL "" FORCE)
FetchContent_MakeAvailable(googletest)

# Create the C test executable
add_executable(c_test src/fff_test_c.c ${COMMON_FILE_LIST})
target_include_directories(c_test PRIVATE include)
target_link_libraries(c_test PRIVATE fff)

# Create the C++ test executable
add_executable(cpp_test src/fff_test_cpp.cpp ${COMMON_FILE_LIST})
target_include_directories(cpp_test PRIVATE include)
target_link_libraries(cpp_test PRIVATE GTest::gtest_main fff)

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
target_link_libraries(cpp_global_test PRIVATE GTest::gtest_main fff)

# Create the C++ custom function signature executable
add_executable(cpp_custom_fn_signature_test
    src/fff_test_custom_function_template.cpp
    ${COMMON_FILE_LIST}
)
target_include_directories(cpp_custom_fn_signature_test PRIVATE include)
target_link_libraries(cpp_custom_fn_signature_test PRIVATE GTest::gtest_main fff)
# Due to a bug in WinLibs for Windows it's not currently possible to use:
#   target_precompile_headers(cpp_custom_fn_signature_test PUBLIC include/custom_function.hpp)
# See more info at target_precompile_headers(cpp_custom_fn_signature_test PUBLIC include/custom_function.hpp)

add_test(
      NAME cpp_custom_fn_signature_test
      COMMAND $<TARGET_FILE:cpp_custom_fn_signature_test>
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