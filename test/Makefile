
BUILD_DIR = ../build

CPP_SRCS += \
fff_test.cpp 

OBJS += \
$(BUILD_DIR)/fff_test.o 

LIBS := -lgtest_main -lgtest -lpthread

PROGNAME = $(BUILD_DIR)/fff_test

# All Target
all: $(BUILD_DIR)/fff_test


# Each subdirectory must supply rules for building sources it contributes
$(BUILD_DIR)/%.o: %.cpp
	@echo 'Building file: $<'
	@echo 'Invoking: GCC C++ Compiler'
	g++ -I/home/mlong/tools/gtest/gtest-1.5.0/include -O0 -g3 -Wall -c -o"$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


# Tool invocations
$(PROGNAME): $(OBJS) $(USER_OBJS)
	@echo 'Building target: $@'
	@echo 'Invoking: GCC C++ Linker'
	g++ -L/home/mlong/tools/gtest -o"$(PROGNAME)" $(OBJS) $(LIBS)
	@echo 'Finished building target: $@'
	@echo ' '

# Other Targets
clean:
	-$(RM) $(OBJS) $(PROGNAME)
	-@echo ' '

