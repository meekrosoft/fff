#pragma once

#include <assert.h>
#include <stdio.h>

#include "fff.h"
#include "bus.fake.h"
#include "error.fake.h"


void init_tests();

extern char GLOBAL_TEST_bus_read_ret[32];
