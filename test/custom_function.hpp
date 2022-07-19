#pragma once

#include <functional>

#define CUSTOM_FFF_FUNCTION_TEMPLATE(RETURN, FUNCNAME, ...) \
  std::function<RETURN (__VA_ARGS__)> FUNCNAME
