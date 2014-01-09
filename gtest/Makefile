
BUILD_DIR = ../build
LIBS := -lpthread
CPP_OBJS=$(BUILD_DIR)/gtest-all.o $(BUILD_DIR)/gtest-main.o

SOURCES=gtest-all.cc gtest-main.cc


# Each subdirectory must supply rules for building sources it contributes
$(BUILD_DIR)/%.o: %.cc
	@echo 'Building file: $<'
	@echo 'Invoking: GCC C++ Compiler'
	g++ -I../ -O0 -g3 -Wall -DGTEST_USE_OWN_TR1_TUPLE=1 -c -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '

all: $(CPP_OBJS)

clean:
	-$(RM) $(CPP_OBJS) 
	-@echo ' '

