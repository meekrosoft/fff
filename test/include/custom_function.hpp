/* Copyright 2022 Google LLC
 * SPDX-License-Identifier: Apache-2.0
 */

#pragma once

#include <functional>

#define CUSTOM_FFF_FUNCTION_TEMPLATE(RETURN, FUNCNAME, ...) \
    std::function<RETURN (__VA_ARGS__)> FUNCNAME
