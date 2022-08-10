# Copyright 2022 Google LLC
# SPDX-License-Identifier: Apache-2.0

# Create the ui_test_ansic test binary
add_executable(ui_test_ansic src/UI_test_ansic.c src/UI.c)
target_include_directories(ui_test_ansic PRIVATE include)
target_link_libraries(ui_test_ansic PRIVATE fff)

# Create the ui_test_cpp test binary
add_executable(ui_test_cpp src/UI_test_cpp.cpp src/UI.c)
target_include_directories(ui_test_cpp PRIVATE include)
target_link_libraries(ui_test_cpp PRIVATE gtest fff)

# Add tests to ctest
add_test(
    NAME ui_test_ansic
    COMMAND $<TARGET_FILE:ui_test_ansic>
)

add_test(
    NAME ui_test_cpp
    COMMAND $<TARGET_FILE:ui_test_cpp>
)
