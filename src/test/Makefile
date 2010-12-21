
CPP_SRCS += \
fff_test.cpp 

OBJS += \
../../build/fff_test.o 

LIBS := -lgtest_main -lgtest -lpthread

# All Target
all: ../../build/fff_test


# Each subdirectory must supply rules for building sources it contributes
../../build/%.o: %.cpp
	@echo 'Building file: $<'
	@echo 'Invoking: GCC C++ Compiler'
	g++ -I/home/mlong/tools/gtest/gtest-1.5.0/include -O0 -g3 -Wall -c -o"$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


# Tool invocations
../../build/fff_test: $(OBJS) $(USER_OBJS)
	@echo 'Building target: $@'
	@echo 'Invoking: GCC C++ Linker'
	g++ -L/home/mlong/tools/gtest -o"../../build/fff_test" $(OBJS) $(LIBS)
	@echo 'Finished building target: $@'
	@echo ' '

# Other Targets
clean:
	-$(RM) $(OBJS) fff_test
	-@echo ' '

