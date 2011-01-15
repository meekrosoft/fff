
BUILD_DIR = ../build

CPP_OBJS += \
$(BUILD_DIR)/fff_test_cpp.o

C_OBJS += \
$(BUILD_DIR)/fff_test_c.o 

LIBS := -lgtest_main -lgtest -lpthread

CPPNAME = $(BUILD_DIR)/fff_test_cpp
CNAME = $(BUILD_DIR)/fff_test_c
# All Target
all: $(CPPNAME) $(CNAME)


# Each subdirectory must supply rules for building sources it contributes
$(BUILD_DIR)/%.o: %.cpp
	@echo 'Building file: $<'
	@echo 'Invoking: GCC C++ Compiler'
	g++ -I/home/mlong/tools/gtest/gtest-1.5.0/include -O0 -g3 -Wall -c -o"$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '

$(BUILD_DIR)/%.o: %.c
	@echo 'Building file: $<'
	@echo 'Invoking: GCC C++ Compiler'
	gcc -I/home/mlong/tools/gtest/gtest-1.5.0/include -O0 -g3 -Wall -std=c99 -c -o"$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


# Tool invocations
$(CPPNAME): $(CPP_OBJS) 
	@echo 'Building target: $@'
	@echo 'Invoking: GCC C++ Linker'
	g++ -L/home/mlong/tools/gtest -o"$(CPPNAME)" $(CPP_OBJS) $(LIBS)
	@echo 'Finished building target: $@'
	@echo ' '

$(CNAME): $(C_OBJS) 
	@echo 'Building target: $@'
	@echo 'Invoking: GCC C++ Linker'
	g++ -L/home/mlong/tools/gtest -o"$(CNAME)" $(C_OBJS) $(LIBS)
	@echo 'Finished building target: $@'
	@echo ' '

# Other Targets
clean:
	-$(RM) $(CPP_OBJS) $(C_OBJS) $(CPPNAME) $(CNAME)
	-@echo ' '

