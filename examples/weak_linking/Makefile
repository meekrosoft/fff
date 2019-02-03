BUILD_DIR = ../../build
INCLUDE_DIRS = -I "../../" -I "./src/"

BUILD_DIR_FAKE =  $(BUILD_DIR)/weak_linking
CC = gcc
WEAK_FLAGS=-Wall -DFFF_GCC_FUNCTION_ATTRIBUTES="__attribute__((weak))"

$(BUILD_DIR_FAKE)/%.o: test/%.c
	@echo "Compiling "$@
	$(CC) $(WEAK_FLAGS) $(INCLUDE_DIRS) -g -O0 -c $< -o $@

FAKE_OBJECTS = $(BUILD_DIR_FAKE)/display.fake.o $(BUILD_DIR_FAKE)/sensor.fake.o $(BUILD_DIR_FAKE)/sensor.fake.o $(BUILD_DIR_FAKE)/error.fake.o  $(BUILD_DIR_FAKE)/bus.fake.o $(BUILD_DIR_FAKE)/test_common.o

TEST_BINARIES = $(BUILD_DIR_FAKE)/test_main $(BUILD_DIR_FAKE)/test_display $(BUILD_DIR_FAKE)/test_sensor
mkdir:
	mkdir -p $(BUILD_DIR_FAKE)/

clean:
	rm -rf $(BUILD_DIR_FAKE)/

$(BUILD_DIR_FAKE)/libfakes.a:  $(FAKE_OBJECTS)
	ar r $@  $^

# First case where we need __weak__ linking:
#  - If we have the build objects (for some reason) in order where the fake object comes first.
$(BUILD_DIR_FAKE)/test_display: ./test/display.test.c $(BUILD_DIR_FAKE)/libfakes.a ./src/display.c
	$(CC) $(WEAK_FLAGS) $(INCLUDE_DIRS) -o $@ $^

# Second case where we need weak linking:
#  - If we use an object from the fake object -> gcc linker will include it.
$(BUILD_DIR_FAKE)/test_sensor: ./test/sensor.test.c ./src/sensor.c $(BUILD_DIR_FAKE)/libfakes.a
	$(CC) $(WEAK_FLAGS) $(INCLUDE_DIRS) -o $@ $^

# Third case where we need weak linking:
#  - We want to fake one function but not all.
$(BUILD_DIR_FAKE)/test_main: ./test/main.test.c ./src/main.c $(BUILD_DIR_FAKE)/libfakes.a
	$(CC) $(WEAK_FLAGS) $(INCLUDE_DIRS) -o $@ $^

all: mkdir $(TEST_BINARIES)
