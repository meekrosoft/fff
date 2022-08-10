# Copyright 2022 Google LLC
# SPDX-License-Identifier: Apache-2.0

# Skip these tests for Windows
if(WIN32)
    message(STATUS "Weak linking requires __attribute__((weak)) which isn't supported on MS VS, skipping tests")
    return()
endif()

# Set the global FFF_GCC_FUNCTION_ATTRIBUTES for config.h
set(FFF_GCC_FUNCTION_ATTRIBUTES "__attribute__((weak))")
configure_file(config/config.h.in ${CMAKE_CURRENT_BINARY_DIR}/config.h)

# Create a libfakes static library that will be used in the executables below.
# This library will depend on the above generated config.h which will add the
# FFF 'weak' function attributes.
add_library(libfakes STATIC
    test/src/bus.fake.c
    test/src/display.fake.c
    test/src/error.fake.c
    test/src/sensor.fake.c
    test/src/test_common.c
)
target_precompile_headers(libfakes PUBLIC ${CMAKE_CURRENT_BINARY_DIR}/config.h)
target_include_directories(libfakes PUBLIC include test/include)
target_link_libraries(libfakes PUBLIC fff)

# Create the main test binary
add_executable(test_main src/main.c test/src/main.test.c)
target_link_libraries(test_main PRIVATE libfakes)

# Create the sensor test binary
add_executable(test_sensor src/sensor.c test/src/sensor.test.c)
target_link_libraries(test_sensor PRIVATE libfakes)

# Create the display test binary
add_executable(test_display src/display.c test/src/display.test.c ${LIBFAKES_SRCS})
target_link_libraries(test_display PRIVATE libfakes)

# Add tests to ctest
add_test(
    NAME test_main
    COMMAND $<TARGET_FILE:test_main>
)

add_test(
    NAME test_sensor
    COMMAND $<TARGET_FILE:test_sensor>
)

add_test(
    NAME test_display
    COMMAND $<TARGET_FILE:test_display>
)
