
BUILD_DIR = ../build

CPP_OBJS += \
$(BUILD_DIR)/fff_test_cpp.o \
$(BUILD_DIR)/gtest-all.o \
$(BUILD_DIR)/gtest-main.o

CTEST_OBJS = $(BUILD_DIR)/fff_test_c.o

GLOBAL_C_OBJS += \
$(BUILD_DIR)/global_fakes.o \
$(BUILD_DIR)/fff_test_global_c.o

LIBS := -lpthread

CPPNAME = $(BUILD_DIR)/fff_test_cpp
CNAME = $(BUILD_DIR)/fff_test_c
GLOBALCNAME = $(BUILD_DIR)/fff_test_glob_c

# All Target
all: $(CPPNAME) $(CNAME) $(GLOBALCNAME)


# Each subdirectory must supply rules for building sources it contributes
$(BUILD_DIR)/%.o: %.cpp
	@echo 'Building file: $<'
	@echo 'Invoking: GCC C++ Compiler'
	g++ -I../ -O0 -g3 -Wall -c -o"$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '

$(BUILD_DIR)/%.o: %.c
	@echo 'Building file: $<'
	@echo 'Invoking: GCC C Compiler'
	gcc -I../ -O0 -g3 -Wall -std=c99 -c -o"$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


# Tool invocations
$(CPPNAME): $(CPP_OBJS) 
	@echo 'Building target: $@'
	@echo 'Invoking: GCC C++ Linker'
	g++ -o "$(CPPNAME)" $(CPP_OBJS) $(LIBS)
	@echo 'Finished building target: $@'
	@echo ' '

$(CNAME): $(CTEST_OBJS) 
	@echo 'Building target: $@'
	@echo 'Invoking: GCC C Linker'
	gcc  -o "$(CNAME)" $(CTEST_OBJS) $(LIBS)
	@echo 'Finished building target: $@'
	@echo ' '

$(GLOBALCNAME): $(GLOBAL_C_OBJS) 
	@echo 'Building target: $@'
	@echo 'Invoking: GCC C++ Linker'
	g++  -o "$(GLOBALCNAME)" $(GLOBAL_C_OBJS) $(LIBS)
	@echo 'Finished building target: $@'
	@echo ' '

# Other Targets
clean:
	-$(RM) $(CPP_OBJS) $(GLOBAL_C_OBJS) $(C_OBJS) $(CPPNAME) $(CNAME) $(GLOBALCNAME)
	-@echo ' '

