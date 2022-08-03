# Copyright 2022 Google LLC
# SPDX-License-Identifier: Apache-2.0

# Create the driver test binary
add_executable(driver_test
    src/driver.c
    src/driver.test.cpp
)
target_include_directories(driver_test PRIVATE include)
target_link_libraries(driver_test PRIVATE gtest fff)
target_compile_definitions(driver_test PUBLIC TEST_USER_OWN_TR1_TUPLE=1 TESTING)

# Create the driver fff test binary
add_executable(driver_fff_test
    src/driver.c
    src/driver.test.fff.cpp
)
target_include_directories(driver_fff_test PRIVATE include)
target_link_libraries(driver_fff_test PRIVATE gtest fff)
target_compile_definitions(driver_fff_test PUBLIC TEST_USER_OWN_TR1_TUPLE=1 TESTING)

# Add tests to ctest
add_test(
    NAME driver_test
    COMMAND $<TARGET_FILE:driver_test>
)

add_test(
    NAME driver_fff_test
    COMMAND $<TARGET_FILE:driver_fff_test>
)
