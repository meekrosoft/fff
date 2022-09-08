/* Copyright 2022 Google LLC
 * SPDX-License-Identifier: Apache-2.0
 */

#include <gtest/gtest.h>

#include "fff_wrapper.hpp"

DEFINE_FFF_GLOBALS

FAKE_VOID_FUNC(do_stuff, int);

class FFFCustomFakeSuite : public ::testing::Test {
 public:
  void SetUp() override {
    RESET_FAKE(do_stuff);
    FFF_RESET_HISTORY();
  }
};

TEST_F(FFFCustomFakeSuite, custom_cpp_fake_function)
{
  int x = 0;
  do_stuff_fake.custom_fake = [&x](int i) {
    x = i;
  };
  do_stuff(5);
  ASSERT_EQ(5, x);
}
