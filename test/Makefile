
BUILD_DIR = ../build

FFF_TEST_CPP_OBJS += \
$(BUILD_DIR)/fff_test_cpp.o \
$(BUILD_DIR)/gtest-all.o \
$(BUILD_DIR)/gtest-main.o

FFF_TEST_GLOBAL_CPP_OBJS += \
$(BUILD_DIR)/fff_test_global_cpp.o \
$(BUILD_DIR)/global_fakes.o \
$(BUILD_DIR)/gtest-all.o \
$(BUILD_DIR)/gtest-main.o

FFF_TEST_C_OBJS = $(BUILD_DIR)/fff_test_c.o

FFF_TEST_GLOBAL_C_OBJS += \
$(BUILD_DIR)/global_fakes.o \
$(BUILD_DIR)/fff_test_global_c.o

FFF_TEST_CPP_TARGET = $(BUILD_DIR)/fff_test_cpp
FFF_TEST_C_TARGET = $(BUILD_DIR)/fff_test_c
FFF_TEST_GLOBAL_C_TARGET = $(BUILD_DIR)/fff_test_glob_c
FFF_TEST_GLOBAL_CPP_TARGET = $(BUILD_DIR)/fff_test_glob_cpp

LIBS := -lpthread
# All Target
all: $(FFF_TEST_CPP_TARGET) $(FFF_TEST_C_TARGET) $(FFF_TEST_GLOBAL_C_TARGET) $(FFF_TEST_GLOBAL_CPP_TARGET)


# Each subdirectory must supply rules for building sources it contributes
$(BUILD_DIR)/%.o: %.cpp
	@echo 'Building file: $<'
	@echo 'Invoking: GCC C++ Compiler'
	g++ -I../ -O0 -g3 -Wall -DGTEST_USE_OWN_TR1_TUPLE=1 -c -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '

$(BUILD_DIR)/%.o: %.c
	@echo 'Building file: $<'
	@echo 'Invoking: GCC C Compiler'
	gcc -I../ -O0 -g3 -Wall -std=c99 -c -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


# Link targets
$(FFF_TEST_CPP_TARGET): $(FFF_TEST_CPP_OBJS) 
	@echo 'Building target: $@'
	@echo 'Invoking: GCC C++ Linker'
	g++ -o "$(FFF_TEST_CPP_TARGET)" $(FFF_TEST_CPP_OBJS) $(LIBS)
	@echo 'Finished building target: $@'
	@echo ' '

$(FFF_TEST_C_TARGET): $(FFF_TEST_C_OBJS) 
	@echo 'Building target: $@'
	@echo 'Invoking: GCC C Linker'
	gcc  -o "$(FFF_TEST_C_TARGET)" $(FFF_TEST_C_OBJS) $(LIBS)
	@echo 'Finished building target: $@'
	@echo ' '

$(FFF_TEST_GLOBAL_C_TARGET): $(FFF_TEST_GLOBAL_C_OBJS) 
	@echo 'Building target: $@'
	@echo 'Invoking: GCC C++ Linker'
	g++  -o "$(FFF_TEST_GLOBAL_C_TARGET)" $(FFF_TEST_GLOBAL_C_OBJS) $(LIBS)
	@echo 'Finished building target: $@'
	@echo ' '

$(FFF_TEST_GLOBAL_CPP_TARGET): $(FFF_TEST_GLOBAL_CPP_OBJS) 
	@echo 'Building target: $@'
	@echo 'Invoking: GCC C++ Linker'
	g++  -o "$(FFF_TEST_GLOBAL_CPP_TARGET)" $(FFF_TEST_GLOBAL_CPP_OBJS) $(LIBS)
	@echo 'Finished building target: $@'
	@echo ' '

# Other Targets
clean:
	-$(RM) $(FFF_TEST_CPP_OBJS) $(FFF_TEST_GLOBAL_C_OBJS) $(FFF_TEST_C_OBJS) \
		$(FFF_TEST_CPP_TARGET) $(FFF_TEST_C_TARGET) $(FFF_TEST_GLOBAL_CPP_TARGET) $(FFF_TEST_GLOBAL_C_TARGET)
	-@echo ' '

